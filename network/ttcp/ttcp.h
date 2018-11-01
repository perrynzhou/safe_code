/*************************************************************************
  > File Name: ttcp.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 三 10/31 10:16:33 2018
 ************************************************************************/

#ifndef _TTCP_H
#define _TTCP_H
#include <stdio.h>
#include <stdint.h>
typedef struct session_msg {
  int32_t number;
  int32_t length;
}session_msg;

typedef struct payload_msg {
  int32_t length;
  char data[0];
}payload_msg;

void get_sock_info(int fd, char *str);
int wirte_n(int fd,const void *buf,size_t len);
int read_n(int fd,void *buf,size_t len);
#endif
