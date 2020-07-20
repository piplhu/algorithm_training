/**
 * @file process_pool.h
 * @author pipl (huhui@hupipl.cn)
 * @brief 进程池多进程处理网络连接
 * @version 0.1
 * @date 2020-07-16
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef _H_PROCESS_POLL_H
#define _H_PROCESS_POLL_H

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int g_sig_pipefd[2];

/**
 * @brief 设置描述符为非阻塞
 *
 * @param fd
 * @return int 以前的描述符选项
 */
static int Setnonblocking(int fd) {
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
static void Addfd(int epollfd, int fd) {
    epoll_event event;
    event.data.fd = fd;
    event.events  = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    Setnonblocking(fd);
}

/**
 * @brief 从epoll事件中移除fd并关闭
 *
 * @param epollfd
 * @param fd
 */
static void Removefd(int epollfd, int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

/**
 * @brief 信号处理：将信号写入管道由epoll接管触发统一处理
 * 
 * @param sig 
 */
static void Sighandler(int sig) {
    int save_errno = errno;
    int msg        = sig;
    send(g_sig_pipefd[1], (char *)&msg, 1, 0);
    errno = save_errno;
}

/**
 * @brief 添加信号处理
 * 
 * @param sig 
 * @param handler 
 * @param restart 
 */
static void Addsig(int sig,void(handler)(int),bool restart = true){
  struct sigaction sa;
  memset(&sa,0,sizeof(sa));
  sa.sa_handler = handler;
  if(restart)
    sa.sa_flags |= SA_RESTART;
  
  sigfillset(&sa.sa_mask);
  assert(sigaction(sig,&sa,NULL) != -1);
}

/**
 * @brief 
 *  描述一个子进程的类
 */
class Process {
  public:
    pid_t pid_ = -1; //子进程PID
    int   pipefd[2]; //父进程和子进程通讯用的管道
  public:
    Process(){};
    ~Process(){};
};

template <typename T, int Num, int FD> class ProcessPool {
  private:
    /**
     * @brief Construct a new Process Pool object
     *
     * @param listenfd
     * @param process_num
     */
    ProcessPool(int listenfd, int process_num);

  public:
    static ProcessPool<T, Num, FD> *GetInstance(int fd) {
        if (!instance_) {
            instance_ = new ProcessPool<T, Num, FD>(fd, Num);
        }
        return instance_;
    }

    ~ProcessPool(){};
    /**
     * @brief 启动进程池
     *
     */
    void Run();

    
  private:
    /**
     * @brief 统一事件源
     *
     */
    void SetupSigPipe();

    /**
     * @brief 父进程流程
     *
     */
    void RunParent();

    /**
     * @brief 子进程流程
     *
     */
    void RunChild();

  private:
    static ProcessPool<T, Num, FD> *instance_;

    static const int MAX_PROCESS_NUMBER = 16; //进程允许的最大子进程数
    static const int USE_PER_PROCESS = 10; //每个子进程最多能处理的客户数量
    static const int MAX_EVENT_NUMBER = 1000; // epoll最多处理的客户数量
    int              process_num_;            //进程池中的进程总数
    int              index_ = -1;             //子进程在池中的序号
    int              epollfd_;                // epool
    int              listenfd_;               //监听socket
    bool             stop_ = false;           //子进程是否停止运行
    Process *        sub_processs_;            //子进程描述信息
};

template <typename T, int Num, int FD>
ProcessPool<T, Num, FD> *ProcessPool<T, Num, FD>::instance_ = nullptr;

template <typename T, int Num, int FD>
ProcessPool<T, Num, FD>::ProcessPool(int listenfd, int process_num)
    : listenfd_(listenfd)
    , process_num_(process_num) {
    assert(process_num > 0 && process_num <= MAX_PROCESS_NUMBER);
    sub_processs_ = new Process[process_num];
    assert(sub_processs_);

    for (int i = 0; i < process_num; i++) {
        int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, sub_processs_[i].pipefd);
        assert(ret == 0);
        sub_processs_[i].pid_ = fork();
        assert(sub_processs_[i].pid_ >= 0);
        if (sub_processs_[i].pid_ > 0) {
            close(sub_processs_[i].pipefd[1]);
            continue;
        } else {
            close(sub_processs_[i].pipefd[0]);
            index_ = i;
            break;
        }
    }
}

template <typename T, int Num, int FD>
void ProcessPool<T, Num, FD>::SetupSigPipe() {
    epollfd_ = epoll_create(5);
    assert(epollfd_ != -1);

    int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, g_sig_pipefd);
    assert(ret != -1);

    Setnonblocking(g_sig_pipefd[1]);
    Addfd(epollfd_, g_sig_pipefd[0]);

    Addsig(SIGCHLD, Sighandler);
    Addsig(SIGTERM, Sighandler);
    Addsig(SIGINT, Sighandler);
    Addsig(SIGPIPE, SIG_IGN);
}

template <typename T, int Num, int FD> 
void ProcessPool<T, Num, FD>::Run(){
    if(index_ != -1){
        RunChild();
        return;
    }
    RunParent();
}

template <typename T, int Num, int FD> 
void ProcessPool<T, Num, FD>::RunChild() {
    SetupSigPipe();

    int pipefd = sub_processs_[index_].pipefd[1];
    Addfd(epollfd_, pipefd);

    epoll_event events[MAX_EVENT_NUMBER];
    T *         users = new T[USE_PER_PROCESS];
    assert(users);

    int number = 0;
    int ret    = -1;

    while (!stop_) {
        number = epoll_wait(epollfd_, events, MAX_EVENT_NUMBER, -1);
        if (number < 0 && errno != EINTR) {
            std::cout << "epoll fail!" << std::endl;
            break;
        }

        for (int i = 0; i < number; i++) {
            int sockfd = events[i].data.fd;
            if (sockfd == pipefd && (events[i].events & EPOLLIN)) {
                int client = 0;
                //新的客户端连接通过父子进程管道通知
                ret = recv(sockfd, (char *)&client, sizeof(client), 0);
                if ((ret < 0 && errno != EAGAIN) || ret == 0)
                    continue;
                else {
                    struct sockaddr_in clientAddress;
                    socklen_t          clientAddrLenght = sizeof(clientAddress);
                    int                connfd =
                        accept(listenfd_, (struct sockaddr *)&clientAddress,
                               &clientAddrLenght);
                    if (connfd < 0) {
                        std::cout << "errno is: " << errno << std::endl;
                        continue;
                    }
                    Addfd(epollfd_, connfd);

                    //初始化模板
                    users[connfd].init(epollfd_, connfd, clientAddress);
                }
            } else if (sockfd == g_sig_pipefd[0] &&
                       (events[i].events & EPOLLIN)) { //处理子进程接受的信号
                int  sig;
                char signals[1024];
                ret = recv(g_sig_pipefd[0], signals, sizeof(signals), 0);
                if (ret <= 0)
                    continue;
                else {
                    for (int i = 0; i < ret; i++) {
                        switch (signals[i]) {
                        case SIGCHLD: {
                            pid_t pid;
                            int   stat;
                            while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
                                continue;

                            break;
                        }
                        case SIGTERM:
                        case SIGINT: {
                            stop_ = true;
                            break;
                        }
                        }
                    }
                }
            } else if (events[i].events &
                       EPOLLIN) { //如果是其他的可读数据，必然是客户端的请求
                users[sockfd].process();
            } else {
                continue;
            }
        }
    }
    delete[] users;
    users = NULL;
    close(pipefd);
    close(epollfd_);
}

template <typename T, int Num, int FD>
void ProcessPool<T, Num, FD>::RunParent() {
    SetupSigPipe();

    Addfd(epollfd_, listenfd_);

    epoll_event events[MAX_EVENT_NUMBER];
    int         subProcessCounter = 0;
    int         new_conn          = 1;
    int         number            = 0;
    int         ret               = -1;
    while (!stop_) {
        number = epoll_wait(epollfd_, events, MAX_EVENT_NUMBER, -1);
        if (number < 0 && errno != EINTR) {
            std::cout << "epoll fail!" << std::endl;
            break;
        }
        for (int i = 0; i < number; i++) {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd_) {
                //如果有新的连接到来，就采用Round Robin方式将其分配给一个子进程
                int i = subProcessCounter;
                do {
                    if (sub_processs_[i].pid_ != -1) break;

                    i = (i + 1) % process_num_;
                } while (i != subProcessCounter);

                if (sub_processs_[i].pid_ == -1) {
                    stop_ = true;
                    break;
                }

                subProcessCounter = (i + 1) % process_num_;
                send(sub_processs_[i].pipefd[0], (char *)&new_conn,
                     sizeof(new_conn), 0);
                std::cout << "send request to child " << i << std::endl;
            } else if (sockfd == g_sig_pipefd[0] &&
                       (events[i].events & EPOLLIN)) { //父进程接收到信号
                int  sig;
                char signals[1024];
                ret = recv(g_sig_pipefd[0], signals, sizeof(signals), 0);
                if (ret < 0)
                    continue;
                else {
                    for (int i = 0; i < ret; i++) {
                        switch (signals[i]) {
                        case SIGCHLD: {
                            pid_t pid;
                            int   stat;
                            while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
                                for (int j = 0; j < process_num_; i++) {
                                    if (sub_processs_[j].pid_ = pid) {
                                        std::cout << "child join :" << i
                                                  << std::endl;
                                        close(sub_processs_[i].pipefd[0]);
                                        sub_processs_[i].pid_ = -1;
                                    }
                                }
                            }
                            // 如果所有的子进程都已经退出了，则父进程也退出
                            stop_ = true;
                            for (int i = 0; i < process_num_; i++) {
                                if (sub_processs_[i].pid_ != -1) stop_ = false;
                            }
                            break;
                        }
                        case SIGTERM:
                        case SIGINT: {
                            std::cout << "kill all the child now!" << std::endl;
                            for (int i = 0; i < process_num_; i++) {
                                int pid = sub_processs_[i].pid_;
                                if (pid != -1) kill(pid, SIGTERM);
                            }
                            break;
                        }
                        }
                    }
                }
            } else {
                continue;
            }
        }
    }
    close(epollfd_);
}

#endif//_H_PROCESS_POLL_H
