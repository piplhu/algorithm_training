/**
 * @file web_test.cpp
 * @author pipl (huhui@hupipl.cn)
 * @brief 测试程序
 * @version 0.1
 * @date 2020-08-07
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <arpa/inet.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "http_conn.h"
#include "thread_pool.hpp"

#define MAX_FD 65536
#define MAX_EVENT_NUMBER 10000

extern int addfd(int epollfd, int fd, bool one_shot);
extern int removefd(int epollfd, int fd);

void addsig(int sig, void(handler)(int), bool restart = true) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if (restart) {
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

void show_error(int connfd, const char *info) {
    std::cout << info << std::endl;
    send(connfd, info, strlen(info), 0);
    close(connfd);
}

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        printf("usage: %s ip_address port \n", basename(argv[0]));
        return 1;
    }

    const char *ip   = argv[1];
    int         port = atoi(argv[2]);

    addsig(SIGPIPE, SIG_IGN);

    ThreadPool<HttpConn> *pool = new ThreadPool<HttpConn>(8,10000);

    HttpConn *user       = new HttpConn[MAX_FD];
    int       user_count = 0;

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);
    struct linger tmp = {1, 0};
    setsockopt(listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));

    int                ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    ret = bind(listenfd, (sockaddr *)&address, sizeof(address));
    assert(ret >= 0);

    ret = listen(listenfd, 5);
    assert(ret >= 0);

    epoll_event events[MAX_EVENT_NUMBER];
    int         epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd, listenfd, false);
    HttpConn::epollfd_ = epollfd;

    while (true) {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if (number < 0 && errno != EINTR) {
            printf("epoll failure\n");
            break;
        }

        for (int i = 0; i < number; i++) {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd) {
                sockaddr_in client_addr;
                socklen_t   client_addlength = sizeof(client_addr);
                int         connfd = accept(listenfd, (sockaddr *)&client_addr,
                                    &client_addlength);
                if (connfd < 0) {
                    printf("errno is %d \n", errno);
                    continue;
                }

                if (HttpConn::user_count_ >= MAX_FD) {
                    show_error(connfd, "Internal server busy");
                    continue;
                }

                user[connfd].init(connfd, client_addr);
            } else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                user[sockfd].close_conn();
            } else if (events[i].events & EPOLLIN) {
                if (user[sockfd].read()) {
                    pool->Append(user + sockfd);
                } else {
                    user[sockfd].close_conn();
                }
            } else if (events[i].events & EPOLLOUT) {
                if (!user[sockfd].write()) {
                    user[sockfd].close_conn();
                }
            } else {
            }
        }
    }
    close(epollfd);
    close(listenfd);
    delete[] user;
    delete pool;
    return 0;
}