/**
 * @file server.c
 * @author pipl (huhui@hupipl.cn)
 * @brief 服务端程序
 * @version 0.1
 * @date 2020-07-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/un.h>
#include <stddef.h>

#include "common.h"

#define SLEEPTIME 3
#define ARGLEN 20
#define MODE S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH /* -rwxr--r-- */
#define PERMIT O_RDWR | O_APPEND | O_CREAT /* if the file not exit ,creat it , data written to it append */

int main(int argc, char *argv) {
    int     status, sockfd[2];
    char    permit[ARGLEN];
    char    mode[ARGLEN];
    char    argsockfd[ARGLEN];
    int     recvfd;
    char    data[20];
    int     bytes;
    int     ret, i;
    ssize_t size;
    int     buflen;
    pid_t   pid, chldpid;

    /* 以下几行是使用域套接字必要变量 */
    int                fdsock, fdaccept;
    struct sockaddr_un addr_server;
    int                len;
    const char path[] = "/home/pipl/foo.socket";

    /* 以下是父进程写入文件的内容 */
    char buf[] = "paraent process write \n";

    /* 父进程同时向处理向client发送的数据 */
    char datasend[] = "send by myopen\n";

    memset(permit, '\0', sizeof(permit));
    memset(mode, '\0', sizeof(mode));
    memset(argsockfd, '\0', sizeof(argsockfd));
    memset(data, '\0', sizeof(data));

    printf("now it is parent process,now will fork a child process\n");
    sleep(SLEEPTIME);

    /* 设置文件权限和打开模式 */
    snprintf(permit, sizeof(permit), "%d", PERMIT);
    snprintf(mode, sizeof(mode), "%d", MODE);
    // printf("in myopen %s, %s\n", permit, mode);

    /* 建立和子进程通信的socket套接字管道 */
    ret = socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd);
    if (0 > ret) {
        printf("socketpair failed,errno is %d \n", errno);
    }

    /* fork 子进程 */
    if (0 == (chldpid = fork())) /* child process */
    {
        printf("now it is child process, sendsock is %d\n", sockfd[1]);
        close(sockfd[0]);
        snprintf(argsockfd, sizeof(argsockfd), "%d", sockfd[1]);

        /* 子进程中执行新程序openfile */
        execl("./openfile", "openfile", permit, mode, argsockfd,
              (char *)NULL);
        printf("execl failed, perimit is %s, mode is %s\n", permit, mode);
        exit(-1);
    }

    /* paraent process start to write the file opened by child process */

    printf("now it is parent process\n");
    close(sockfd[1]);
    bytes = sizeof(data);

    /* 等待子进程结束 */
    pid = wait(&status);
    if ((status = WEXITSTATUS(status)) == 0) /* child process terminate */
    {
        printf("child %d process terminate,now parent will write file ...\n",
               pid);
    }

    /* 从子进程取得文件描述符 */
    recvfd = -1;
    // printf("recv sock is %d\n", sockfd[0]);

#if 0
    recvfd = recv_fd(sockfd[0]);
    if (recvfd < 0) {
        printf("paraent recv failed\n");
    }else {
        //  printf("fd %d paraent recv %d bytes data is %s\n", recvfd, strlen(data),
        //         data);
        printf("recv fd: %d \n",recvfd);
    }
#else
    ret = func_recv_fd(sockfd[0], &recvfd, datasend, sizeof(datasend));
    if (ret < 0) {
        printf("paraent recv failed\n");
    }else {
         printf("fd %d paraent recv %d bytes data is %s\n", recvfd, strlen(data),
                data);
    }
#endif

    

    /* 向文件写入数据  */
    size   = -1;
    buflen = strlen(buf);

    if ((size = write(recvfd, buf, buflen)) <= 0) {
        printf("in openfileprog, write failed\n");
    }
    printf("parent write file size:%d - %s \n", buflen, size == buflen ? "true" : "false");
    /* 父进程作为server建立域套接字，等待client连接 */
    printf("parent write over! Accept other process ......\n");

    fdsock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == fdsock) {
        printf("myopen creat socket error!errno is %d\n", errno);
    }

    unlink(path);

    memset(&addr_server, 0, sizeof(addr_server));
    addr_server.sun_family = AF_UNIX;
    strcpy(addr_server.sun_path, path);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(addr_server.sun_path);

    ret = bind(fdsock, (struct sockaddr *)&addr_server, len);
    if (-1 == ret) {
        printf("in myopen bind error, errorno is %d\n", errno);
        close(fdsock);
        unlink(path);
    }

    ret = listen(fdsock, 1);
    if (-1 == ret) {
        printf("in myopen listen error, errorno is %d\n", errno);
        close(fdsock);
        unlink(path);
    }

    fdaccept = accept(fdsock, (struct sockaddr *)&addr_server, &len);
    if (-1 == ret) {
        printf("in myopen accept error, errorno is %d\n", errno);
        close(fdsock);
        unlink(path);
    }

    /* 向已经连接的client传递该文件描述符 */
#if 0
    ret = send_fd(fdaccept, recvfd);
#else
    ret = func_send_fd(fdaccept, recvfd, datasend, sizeof(datasend));
#endif
    
    if (-1 == ret) {
        printf("in myopen, func_send_fd failed\n");
    }

    printf("send fd over! Will sleep 10s \n");
    close(recvfd);
    sleep(10);
    unlink(path);
    exit(0);
}