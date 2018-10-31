/*************************************************************************
  > File Name: tpcc-client.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: 三 10/31 10:14:59 2018
 ************************************************************************/

#include "tpcc.h"
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#define BACKLOG (1024)
int usage(const char *s) {
  fprintf(stdout, "usage: %s {host} {port} {count}");
  return -1;
}
int main(int argc, char *argv[]) {
  if (argc != 4) {
    return usage(argv[0]);
  }
  char *host = argv[1];
  int port = atoi(argv[2]);
  int count = atoi(argv[3]);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    perror("socket happen error");
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
  if (connection(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    close(sock);
    perror("##connect happen error");
    return -1;
  }
  if (write(sock, &sm, sizeof(sm)) != sizeof(sm)) {
    perror("##write happen error");
  }

  payload_msg pm;
  pm.length = sm.length;
  write(sock, &pm, sizeof(pm));
  int32_t val = 0;
  for (int i = 0; i < sm.number; i++) {
    if (write(sock, &sm, sizeof(sm)) == -1) {
      perror("##write happen error");
    }
    if (read(sock, &val, sizeof(val)) == -1) {
      perror("##read happen error");
    }
    char ip[129] = {'\0'};
    get_sock_info(sock, &ip);
    size_t len = strlen(ip);
    if (len > 0) {
      fprintf(stdout, "::client::read from %s,value=%d\n", ip, val);
    }
  }

  close(sock);
}