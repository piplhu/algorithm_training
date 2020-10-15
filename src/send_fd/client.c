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

#include "common.h"

#define SLEEPTIME 3
#define ARGLEN 20

int main() {
    int sockfd, recvfd, ret;

    struct sockaddr_un addr_client;
    int                length, buflen;
    char               data[10];
    ssize_t            size;
    const char path[] = "/home/pipl/foo.socket";
    char       buf[]  = "this is client process\n";

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sockfd) {
        printf("client creat socket error!errno is %d\n", errno);
    }

    addr_client.sun_family = AF_UNIX;
    strcpy(addr_client.sun_path, path);
    length = sizeof(addr_client.sun_family) + sizeof(addr_client.sun_path);

    ret = connect(sockfd, (struct sockaddr *)&addr_client, length);
    if (-1 == ret) {
        printf("in client connect error, errorno is %d\n", errno);
        close(sockfd);
    }

#if 0
    recvfd = recv_fd(sockfd);
    if (-1 == recvfd) {
        printf("in client func_recv_fd failed\n");
        close(sockfd);
    }
    else {
        // printf("fd %d  from server recv  %d bytes data is %s\n", recvfd, strlen(data),
        //        data);
        printf("recv fd: %d \n",recvfd);
    }

#else

    ret = func_recv_fd(sockfd, &recvfd, data, sizeof(data));;
    if (-1 == ret) {
        printf("in client func_recv_fd failed\n");
        close(sockfd);
    }
    else {
        printf("fd %d  from server recv  %d bytes data is %s\n", recvfd,
               strlen(data), data);
    }
#endif
    

    size   = -1;
    buflen = strlen(buf);

    if ((size = write(recvfd, buf, buflen)) <= 0) {
        printf("in openfileprog, write failed\n");
    }

    printf("client write over!\n");

    close(recvfd);
    unlink(path);
    exit(0);
}