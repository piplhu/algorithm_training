/**
 * @file openfile.c
 * @author pipl (huhui@hupipl.cn)
 * @brief 子进程程序
 * @version 0.1
 * @date 2020-07-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"

#define SLEEPTIME 3
#define ARGLEN 20



int main(int argc, char *argv[]){ /* openfileprog */
    if(argc < 4){
        printf( "usage %d  \n",basename(argv[0]));
        return 1;
    }
         
    int     i, fd, ret;
    ssize_t size;
    size_t  buflen;
    char    data[10];
    char buf[] = "openfilep write test \n"; /* 向文件中写入的内容 */
    /* execl("./openfileprog", permit, mode, argsockfd, (char *)NULL); */
    fd = -1;
    if ((fd = open("./file.txt", atoi(argv[1]), atoi(argv[2]))) < 0) {
        printf("in openfileprog, open failed\n");
        exit(-1);
    }

    size   = -1;
    buflen = strlen(buf);
    if ((size = write(fd, buf, buflen)) <= 0) {
        printf("in openfileprog, write failed\n");
    }

    /* 把设定的data信息也传给父进程 */
    ret = 'a';
    for (i = 0; i < sizeof(data); i++, ret++) {
        data[i] = ret;
    }
    data[sizeof(data) - 1] = '\0';

    ret = -1;

#if 0
    ret = send_fd(atoi(argv[3]), fd);
#else
    ret = func_send_fd(atoi(argv[3]), fd, data, 10);
#endif

    if (-1 == ret ) {
        printf("in openfileprog, func_send_fd failed\n");
    }

    close(fd);

    return 0;
}