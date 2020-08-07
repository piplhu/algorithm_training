/**
 * @file http_conn.h
 * @author pipl (huhui@hupipl.cn)
 * @brief http请求处理,限仅支持GET/http1.1
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _H_HTTP_CONN_H_
#define _H_HTTP_CONN_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>

class HttpConn {
  public:
    /* 文件名最大长度 */
    static const int FILENAME_LEN = 200;
    /* 读缓冲区大小*/
    static const int READ_BUFFER_SZIE = 2048;
    /* 写缓冲区大小 */
    static const int WRITE_BUFFER_SZIE = 1024;
    /* http 请求方式 */
    enum METHOD {
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRAACE,
        OPTIONS,
        CONNECT,
        PATCH
    };
    /* 解析客户请求时，主状态机的状态 */
    enum CHECK_STATE {
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADR,
        CHECK_STATE_CONTENT
    };
    /* 服务器处理http请求的可能结果 */
    enum HTTP_CODE {
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION
    };
    /* 行读取状态 */
    enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_OPEN };

  public:
    HttpConn(){};
    ~HttpConn(){};

  public:
    /* 初始化新接受的连接 */
    void init(int sockfd, const sockaddr_in &addr);
    /* 关闭连接 */
    void close_conn(bool real_close = true);
    /* 处理客户请求 */
    void process();
    /* 非阻塞读 */
    bool read();
    /* 非阻塞写 */
    bool write();

  private:
    /* 重置 */
    void init();
    /* 解析http请求 */
    HTTP_CODE process_read();
    /* 填充HTTP应答 */
    bool process_write(HTTP_CODE ret);
    /* 一组被process_read调用分析http请求 */
    HTTP_CODE parse_request_line(char *text);
    HTTP_CODE parse_headers(char *text);
    HTTP_CODE parse_content(char *text);
    HTTP_CODE do_request();
    char *    get_line() {
        return read_buf_ + start_line_;
    };
    LINE_STATUS parse_line();

    /* 一组被process_write调用填充http应答 */
    void unmap();
    bool add_response(const char *format, ...);
    bool add_content(const char *content);
    bool add_status(int status, const char *title);
    bool add_headers(int content_length);
    bool add_content_length(int content_length);
    bool add_linger();
    bool add_blank_line();

  public:
    /* epool */
    static int epollfd_;
    /* 用户数量 */
    static int user_count_;

  private:
    /* 与客户端连接fd和地址 */
    int         sockfd_;
    sockaddr_in address_;

    /* 读缓冲区 */
    char read_buf_[READ_BUFFER_SZIE] = {0};
    /* 已读缓冲区的下个字节位置 */
    int read_index_ = 0;
    /* 当前解析的字符在缓冲区的位置 */
    int checked_index_ = 0;
    /* 当前正在解析的行的起始位置 */
    int start_line_ = 0;

    /* 写缓冲区 */
    char write_buf_[WRITE_BUFFER_SZIE] = {0};
    /* 写缓冲区中代发送的字节 */
    int write_index_ = 0;

    /* 主状态机当前所处的状态 */
    CHECK_STATE check_state_ = CHECK_STATE_REQUESTLINE;
    /* 请求方式 */
    METHOD method_ = GET;

    /* 客户端请求的目标文件的完整路径 */
    char real_file_[FILENAME_LEN] = {0};
    /* 客户请求的目标文件的文件名 */
    char *url_ = nullptr;
    /* HTTP 协议版本号 */
    char *version_ = 0;
    /* 主机名 */
    char *host_ = nullptr;
    /* HTTP 请求的消息体长度 */
    int content_length_ = 0;
    /* HTTP请求是否要求保持连接 */
    bool linger_ = false;

    /* 客户请求的目标文件被mmap带内存中的起始位置 */
    char *file_address_;
    /* 目标文件的状态 */
    struct stat file_stat_;
    /* writev 参数需要的内存块 */
    struct iovec iv_[2];
    int          iv_count_;
};

#endif //_H_HTTP_CONN_H_