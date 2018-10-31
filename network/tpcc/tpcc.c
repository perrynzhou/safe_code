/*************************************************************************
  > File Name: tpcc.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: 三 10/31 12:58:14 2018
 ************************************************************************/

#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
void get_sock_info(int fd, char *str) {
  struct sockaddr ip_addr_struct;
  socklen_t len =  sizeof(ip_addr_struct);
  getpeername(fd, (struct sockaddr *)&ip_addr_struct, &len);
  struct sockaddr_in *s = (struct sockaddr_in *)&ip_addr_struct;
  int port = ntohs(s->sin_port);
  char ip_addr[64] = {'\0'};
  inet_ntop(AF_INET, &s->sin_addr, (char *)&ip_addr, 64);
  sprintf(str, "%s:%d", ip_addr, port);
}