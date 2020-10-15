/**
 * @file common.h
 * @author pipl (huhui@hupipl.cn)
 * @brief  共用发送接收fd接口
 * @version 0.1
 * @date 2020-07-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _H_COMMON_H_
#define _H_COMMON_H_

int func_send_fd(int send_sock, int send_fd, void *data, int bytes);
int func_recv_fd(int recv_sock, int *recvfd, void *data, int bytes);
int send_fd(int fd, int fd_to_send);
int recv_fd(int fd);
#endif //_H_COMMON_H_