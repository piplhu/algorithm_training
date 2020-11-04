#include "common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define	CONTROLLEN	CMSG_LEN(sizeof(int))
static struct cmsghdr	*cmptr = NULL;
#define	MAXLINE	4096
#define RELOP !=

int func_send_fd(int send_sock, int send_fd, void *data, int bytes) {
    struct msghdr msghead;
    struct iovec  lov[1];
    int           ret;

    /* 填充cmsghdr结构 */
    cmptr             = malloc(CONTROLLEN);
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type  = SCM_RIGHTS;
    cmptr->cmsg_len   = CONTROLLEN;

    /* 填充msghead结构 */
    msghead.msg_control      = cmptr;
    msghead.msg_controllen   = CONTROLLEN;
    *(int *)CMSG_DATA(cmptr) = send_fd;

    msghead.msg_name     = NULL;
    msghead.msg_namelen  = 0;
    lov[0].iov_base = data;
    lov[0].iov_len  = bytes;

    msghead.msg_iov    = lov;
    msghead.msg_iovlen = 1;

    /* 发送信息 */
    if (0 > (ret = sendmsg(send_sock, &msghead, 0))) {
        printf("in func_send, send_fd is %d, sendsock is %d, sendmsg "
               "failed,errno is %d\n",
               send_fd, send_sock, errno);
        return -1;
    }

    return ret;
}

int func_recv_fd(int recv_sock, int *recvfd, void *data, int bytes) {
    struct msghdr msghead;
    struct iovec  lov[1];
    int           ret;
    int           temp;
    int           newfd;

    struct cmsghdr *msgptr = NULL;
    msgptr = malloc(CONTROLLEN);
    memset(&msghead, 0, sizeof(msghead));

    /* 同func_send_fd ，填充所需要的结构 */
    msghead.msg_control = msgptr;
    msghead.msg_controllen = CONTROLLEN;

    msghead.msg_name     = NULL;
    msghead.msg_namelen  = 0;
    lov[0].iov_base = data;
    lov[0].iov_len  = bytes;

    msghead.msg_iov    = lov;
    msghead.msg_iovlen = 1;

    /* 接收信息（文件描述符 ）*/
    if (0 > (ret = recvmsg(recv_sock, &msghead, 0))) {
        printf("in func_recv_fd, recvmsg failed\n");
        return -1;
    }

    if ((newfd = *(int*)CMSG_DATA(msgptr)) > 0) {
        *recvfd = newfd; /* 文件描述符 */
    }

    return ret;
}

int send_fd(int fd, int fd_to_send) {
    struct iovec  iov[1];
    struct msghdr msg;
    char          buf[2]; /* send_fd()/recv_fd() 2-byte protocol */

    iov[0].iov_base = buf;
    iov[0].iov_len  = 2;
    msg.msg_iov     = iov;
    msg.msg_iovlen  = 1;
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;

    if (fd_to_send < 0) {
        msg.msg_control    = NULL;
        msg.msg_controllen = 0;
        buf[1]             = -fd_to_send; /* nonzero status means error */
        if (buf[1] == 0) buf[1] = 1; /* -256, etc. would screw up protocol */
    } else {
        if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL) return (-1);
        cmptr->cmsg_level        = SOL_SOCKET;
        cmptr->cmsg_type         = SCM_RIGHTS;
        cmptr->cmsg_len          = CONTROLLEN;
        msg.msg_control          = cmptr;
        msg.msg_controllen       = CONTROLLEN;
        *(int *)CMSG_DATA(cmptr) = fd_to_send; /* the fd to pass */
        buf[1]                   = 0;          /* zero status means OK */
    }

    buf[0] = 0; /* null byte flag to recv_fd() */
    if (sendmsg(fd, &msg, 0) != 2) return (-1);
    return (0);
}

int recv_fd(int fd) {
    int           newfd, nr, status;
    char *        ptr;
    char          buf[MAXLINE];
    struct iovec  iov[1];
    struct msghdr msg;

    status = -1;
    for (;;) {
        iov[0].iov_base = buf;
        iov[0].iov_len  = sizeof(buf);
        msg.msg_iov     = iov;
        msg.msg_iovlen  = 1;
        msg.msg_name    = NULL;
        msg.msg_namelen = 0;
        if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL) return (-1);
        msg.msg_control    = cmptr;
        msg.msg_controllen = CONTROLLEN;
        if ((nr = recvmsg(fd, &msg, 0)) < 0) {
            printf("recvmsg error");
            return (-1);
        } else if (nr == 0) {
            printf("connection closed by server");
            return (-1);
        }

        /*
         * See if this is the final data with null & status.  Null
         * is next to last byte of buffer; status byte is last byte.
         * Zero status means there is a file descriptor to receive.
         */
        for (ptr = buf; ptr < &buf[nr];) {
            if (*ptr++ == 0) {
                if (ptr != &buf[nr - 1]) printf("message format error");
                status = *ptr & 0xFF; /* prevent sign extension */
                if (status == 0) {
                    if (msg.msg_controllen RELOP CONTROLLEN)
                        printf("status = 0 but no fd");
                    newfd = *(int *)CMSG_DATA(cmptr);
                } else {
                    newfd = -status;
                }
                nr -= 2;
            }
        }
        //if (nr > 0 && (*userfunc)(STDERR_FILENO, buf, nr) != nr) return (-1);
        if (status >= 0)    /* final data has arrived */
            return (newfd); /* descriptor, or -status */
    }
}
