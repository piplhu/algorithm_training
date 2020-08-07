#include "http_conn.h"

#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <unistd.h>

namespace {
/* 定义http响应的一些状态信息 */
const char *ok_200_title    = "ok";
const char *error_400_title = "Bad Request";
const char *error_400_form =
    "Your request has bad syntax or is inherently impossible to satisfy.\n";
const char *error_403_title = "Forbidden";
const char *error_403_form =
    "You do not have permission to get file from this server.\n";
const char *error_404_title = "Not Found";
const char *error_404_form =
    "The requested file was not found on this server.\n";
const char *error_500_title = "Internal Error";
const char *error_500_form =
    "There was an unusual problem serving the requested file.\n";
const char *doc_root = "/var/www/html";

} // namespace

/**
 * @brief 设置描述符为非阻塞
 *
 * @param fd
 * @return int 以前的描述符选项
 */
int setnonblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

/**
 * @brief 添加描述符到epoll内核事件表，并设置为非阻塞
 *
 * @param epollfd
 * @param fd
 */
void addfd(int epollfd, int fd, bool one_shot) {
    epoll_event event;
    event.data.fd = fd;
    event.events  = EPOLLIN | EPOLLET | EPOLLRDHUP;
    if (one_shot) event.events |= EPOLLONESHOT;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

/**
 * @brief 从epoll事件中移除fd并关闭
 *
 * @param epollfd
 * @param fd
 */
void removefd(int epollfd, int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

/**
 * @brief 更改fd注册epool参数
 *
 * @param epollfd
 * @param fd
 * @param ev
 */
void modfd(int epollfd, int fd, int ev) {
    epoll_event event;
    event.data.fd = fd;
    event.events  = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

int HttpConn::user_count_ = 0;
int HttpConn::epollfd_    = -1;

void HttpConn::close_conn(bool real_close) {
    if (real_close && sockfd_ != -1) {
        removefd(epollfd_, sockfd_);
        sockfd_ = -1;
        user_count_--;
    }
}

void HttpConn::init(int sockfd, const sockaddr_in &addr) {
    sockfd_  = sockfd;
    address_ = addr;
    addfd(epollfd_, sockfd, true);
    user_count_++;
}

HttpConn::LINE_STATUS HttpConn::parse_line() {
    char temp;
    for (; checked_index_ < read_index_; ++checked_index_) {
        temp = read_buf_[checked_index_];
        if (temp == '\r') {
            if (checked_index_ + 1 == read_index_) {
                return LINE_OPEN;
            } else if (read_buf_[checked_index_ + 1] == '\n') {
                read_buf_[checked_index_++] = '\0';
                read_buf_[checked_index_++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        } else if (temp == '\n') {
            if (checked_index_ > 1 && read_buf_[checked_index_ - 1] == '\r') {
                read_buf_[checked_index_ - 1] = '\0';
                read_buf_[checked_index_++]   = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}

bool HttpConn::read() {
    if (read_index_ >= READ_BUFFER_SZIE) return false;

    int bytes_read = 0;
    while (true) {
        bytes_read = recv(sockfd_, read_buf_ + read_index_,
                          READ_BUFFER_SZIE - read_index_, 0);
        if (bytes_read == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;

            return false;
        } else if (bytes_read == 0) {
            return false;
        }

        read_index_ += bytes_read;
    }
    return true;
}

HttpConn::HTTP_CODE HttpConn::parse_request_line(char *text) {
    url_ = strpbrk(text, " ");
    if (!url_) return BAD_REQUEST;

    *url_++ = '\0';

    char *method = text;
    if (strcasecmp(method, "GET") == 0) {
        method_ = GET;
    } else {
        return BAD_REQUEST;
    }

    url_ += strspn(url_, " ");
    version_ = strpbrk(url_, " ");
    if (!version_) return BAD_REQUEST;

    *version_++ = '\0';
    version_ += strspn(version_, " ");
    if (strcasecmp(version_, "HTTP/1.1") != 0) {
        return BAD_REQUEST;
    }

    if (strncasecmp(url_, "http://", 7) == 0) {
        url_ += 7;
        url_ = strchr(url_, '/');
    }

    if (!url_ || url_[0] != '/') {
        return BAD_REQUEST;
    }

    check_state_ = CHECK_STATE_HEADR;
    return NO_REQUEST;
}

HttpConn::HTTP_CODE HttpConn::parse_headers(char *text) {
    if (text[0] == '\0') {
        if (content_length_ != 0) {
            check_state_ = CHECK_STATE_CONTENT;
            return NO_REQUEST;
        }
    } else if (strncasecmp(text, "Connection:", 11) == 0) {
        text += 11;
        text += strspn(text, "\t");
        if (strcasecmp(text, "keep-alive") == 0) {
            linger_ = true;
        }
    } else if (strncasecmp(text, "Content-Length:", 15) == 0) {
        text += 15;
        text += strspn(text, "\t");
        content_length_ = atol(text);
    } else if (strncasecmp(text, "Host:", 5) == 0) {
        text += 5;
        text += strspn(text, "\t");
        host_ = text;
    } else {
        std::cout << "Unknown header " << text << std::endl;
    }
    return NO_REQUEST;
}

HttpConn::HTTP_CODE HttpConn::parse_content(char *text) {
    if (read_index_ >= content_length_ + checked_index_) {
        text[content_length_] = '\0';
        return GET_REQUEST;
    }
    return NO_REQUEST;
}

HttpConn::HTTP_CODE HttpConn::process_read() {
    LINE_STATUS line_status = LINE_OK;
    HTTP_CODE   ret         = NO_REQUEST;
    char *      text        = 0;
    while ((check_state_ == CHECK_STATE_CONTENT && line_status == LINE_OK) ||
           ((line_status = parse_line()) == LINE_OK)) {
        text        = get_line();
        start_line_ = checked_index_;
        std::cout << "got 1 http line: " << text << std::endl;

        switch (check_state_) {
        case CHECK_STATE_REQUESTLINE: {
            ret = parse_request_line(text);
            if (ret == BAD_REQUEST) return BAD_REQUEST;

            break;
        }
        case CHECK_STATE_HEADR: {
            ret = parse_headers(text);
            if (ret == BAD_REQUEST)
                return BAD_REQUEST;
            else if (ret == GET_REQUEST)
                return do_request();

            break;
        }
        case CHECK_STATE_CONTENT: {
            ret = parse_content(text);
            if (ret == GET_REQUEST) return do_request();

            line_status = LINE_OPEN;
            break;
        }
        default:
            return INTERNAL_ERROR;
        }
    }
    return NO_REQUEST;
}

HttpConn::HTTP_CODE HttpConn::do_request() {
    strcpy(real_file_, doc_root);
    int len = strlen(doc_root);
    strncpy(real_file_ + len, url_, FILENAME_LEN - len - 1);
    if (stat(real_file_, &file_stat_) < 0) return NO_RESOURCE;

    if (!(file_stat_.st_mode & S_IROTH)) return FORBIDDEN_REQUEST;

    if (S_ISDIR(file_stat_.st_mode)) return BAD_REQUEST;

    int fd = open(real_file_, O_RDONLY);
    file_address_ =
        (char *)mmap(0, file_stat_.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    return FILE_REQUEST;
}

void HttpConn::unmap() {
    if (file_address_) {
        munmap(file_address_, file_stat_.st_size);
        file_address_ = 0;
    }
}

void HttpConn::init() {
    check_state_ = CHECK_STATE_REQUESTLINE;
    linger_      = false;

    method_         = GET;
    url_            = 0;
    version_        = 0;
    content_length_ = 0;
    host_           = 0;
    start_line_     = 0;
    checked_index_  = 0;
    read_index_     = 0;
    write_index_    = 0;
    memset(read_buf_, '\0', READ_BUFFER_SZIE);
    memset(write_buf_, '\0', WRITE_BUFFER_SZIE);
    memset(real_file_, '\0', FILENAME_LEN);
}

bool HttpConn::write() {
    
    int temp            = 0;
    int bytes_have_send = 0;
    int bytes_to_send   = write_index_;
    if (bytes_to_send == 0) {
        modfd(epollfd_, sockfd_, EPOLLIN);
        init();
        return true;
    }
    while (1) {
        temp = writev(sockfd_, iv_, iv_count_);
        printf(".......response ...%d...\n",temp);
        if (temp <= -1) {
            if (errno == EAGAIN) {
                modfd(epollfd_, sockfd_, EPOLLOUT);
                return true;
            }
            unmap();
            return false;
        }
        bytes_to_send -= temp;
        bytes_have_send += temp;
        if (bytes_to_send <= bytes_have_send) {
            unmap();
            if (linger_) {
                init();
                modfd(epollfd_, sockfd_, EPOLLIN);
                return true;
            } else {
                init();
                modfd(epollfd_, sockfd_, EPOLLIN);
                return false;
            }
        }
    }
}

bool HttpConn::add_response(const char *format, ...) {
    if (write_index_ >= WRITE_BUFFER_SZIE) return false;

    va_list arglist;
    va_start(arglist, format);
    int len = vsnprintf(write_buf_ + write_index_,
                        WRITE_BUFFER_SZIE - 1 - write_index_, format, arglist);
    if (len >= WRITE_BUFFER_SZIE - 1 - write_index_) {
        return false;
    }

    write_index_ += len;
    va_end(arglist);
    return true;
}

bool HttpConn::add_status(int status, const char *title) {
    return add_response("%s %d %s\r\n", "HTTP/1.1", status, title);
}

bool HttpConn::add_headers(int content_len) {
    add_content_length(content_len);
    add_linger();
    add_blank_line();
}

bool HttpConn::add_content_length(int content_len) {
    return add_response("Content-Length: %d\r\n", content_len);
}

bool HttpConn::add_linger() {
    return add_response("Connection: %s\r\n",
                        linger_ == true ? "keep-alive" : "close");
}

bool HttpConn::add_blank_line() {
    return add_response("%s", "\r\n");
}

bool HttpConn::add_content(const char *content) {
    return add_response("%s", content);
}

bool HttpConn::process_write(HTTP_CODE ret) {
    switch (ret) {
    case INTERNAL_ERROR: {
        add_status(500, error_500_title);
        add_headers(strlen(error_500_form));
        if (!add_content(error_500_form)) return false;
        break;
    }
    case BAD_REQUEST: {
        add_status(400, error_400_title);
        add_headers(strlen(error_403_form));
        if (!add_content(error_400_form)) return false;
        break;
    }
    case NO_RESOURCE: {
        add_status(404, error_404_title);
        add_headers(strlen(error_404_form));
        if (!add_content(error_404_form)) return false;
        break;
    }
    case FORBIDDEN_REQUEST: {
        add_status(403, error_403_title);
        add_headers(strlen(error_403_form));
        if (!add_content(error_403_form)) return false;
        break;
    }
    case FILE_REQUEST: {
        add_status(200, ok_200_title);
        if (file_stat_.st_size != 0) {
            add_headers(file_stat_.st_size);
            iv_[0].iov_base = write_buf_;
            iv_[0].iov_len  = write_index_;
            iv_[1].iov_base = file_address_;
            iv_[1].iov_len  = file_stat_.st_size;
            iv_count_       = 2;
            return true;
        } else {
            const char *ok_string = "<html><body></body></html>";
            add_headers(strlen(ok_string));
            if (!add_content(ok_string)) return false;
            break;
        }
    }
    default:
        return false;
    }
    iv_[0].iov_base = write_buf_;
    iv_[0].iov_len  = write_index_;
    iv_count_       = 1;
    return true;
}

void HttpConn::process() {
    HTTP_CODE read_ret = process_read();
    if (read_ret == NO_REQUEST) {
        modfd(epollfd_, sockfd_, EPOLLIN);
        return;
    }
    bool write_ret = process_write(read_ret);
    if (!write_ret) close_conn();

    modfd(epollfd_, sockfd_, EPOLLOUT);
}