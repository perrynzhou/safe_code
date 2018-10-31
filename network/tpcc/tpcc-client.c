/*************************************************************************
  > File Name: tpcc-client.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: 三 10/31 10:14:59 2018
 ************************************************************************/

#include "tpcc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BACKLOG (1024)
void signal_handle(int signo)
{
}
int usage(const char *s)
{
  fprintf(stdout, "usage: %s {host} {port} {count}");
  return -1;
}
int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    return usage(argv[0]);
  }
  char *host = argv[1];
  int port = atoi(argv[2]);
  int count = atoi(argv[3]);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    printf("socket error: %s(errno: %d)\n", strerror(errno), errno);
    return -1;
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(host);
  addr.sin_port = htons(port);

  session_msg sm;
  sm.number = count;
  sm.length = 8092;
  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
  {
    close(sock);
    printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
    return -1;
  }
  if (send(sock, &sm, sizeof(sm), 0) != sizeof(sm))
  {
    printf("send error: %s(errno: %d)\n", strerror(errno), errno);
  }

  payload_msg pm;
  pm.length = sm.length;
  send(sock, &pm, sizeof(pm), 0);
  int32_t val = 0;

  for (int i = 0; i < sm.number; i++)
  {
    if (send(sock, &pm, sizeof(pm), MSG_NOSIGNAL) == -1)
    {
      printf("send error: %s(errno: %d)\n", strerror(errno), errno);
    }
    if (recv(sock, &val, sizeof(val), 0) == -1)
    {
      printf("recv error: %s(errno: %d)\n", strerror(errno), errno);
    }
    char ip[129] = {'\0'};
    get_sock_info(sock, (char *)&ip);
    size_t len = strlen(ip);
    if (len > 0)
    {
      fprintf(stdout, "::client::read from %s,value=%d\n", ip, val);
    }
  }
  close(sock);
}