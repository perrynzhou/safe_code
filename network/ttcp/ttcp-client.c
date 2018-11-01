/*************************************************************************
  > File Name: ttcp-client.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: 三 10/31 10:14:59 2018
 ************************************************************************/

#include "tpcc.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define BACKLOG (1024)
#define STD_PAGE_SIZE (4096)
void signal_handle(int signo) {}
static void rand_str(char *buf, size_t len) {
  const char o[] = "0123456789ABCDEF";
  size_t olen = sizeof(o) / sizeof(char);
  for (size_t i = 0; i < len; i++) {
    buf[i] = o[(olen + rand()) % olen];
  }
}
int usage(const char *s) {
  fprintf(stdout, "\nusage: %s {host} {port} {block_size} {total_mb_size}\n");
  return -1;
}
int main(int argc, char *argv[]) {
  if (argc != 5) {
    return usage(argv[0]);
  }
  char *host = argv[1];
  int port = atoi(argv[2]);
  int block = (argv[3] == NULL) ? STD_PAGE_SIZE : atoi(argv[3]);
  int bytes = atoi(argv[4]) * 1024 * 1024;
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == -1) {
    printf("socket error: %s(errno: %d)\n", strerror(errno), errno);
    return -1;
  }
  if (block % 8 != 0) {
    block = ((block + 0x7) & ~(0x7);
  }
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(host);
  addr.sin_port = htons(port);

  session_msg sm;
  sm.number = bytes / block_size;
  sm.length = block_size;
  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    close(sock);
    printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
    return -1;
  }
  if (writen(sock, &sm, sizeof(sm)) != sizeof(sm)) {
    printf("send error: %s(errno: %d)\n", strerror(errno), errno);
  }

  payload_msg *pm = (payload_msg *)calloc(1, sizeof(*pm) + block_size);
  pm->length = block_size;
  rand_str(pm->data, pm->length);
  pm->data[pm->length - 1] = '\0';

  double total = (sizeof(char) * pm->length * sm.number) / 1024 / 1024;

  char ip[128] = {'\0'};
  get_sock_info(sock, (char *)&ip);
  size_t len = strlen(ip);

  fprintf(stdout, "------client(%s) write %.3f Mib to server------\n", ip,
          total);

  for (int i = 0; i < sm.number; i++) {
    int write_len = writen(sock, &pm, sizeof(pm));
    assert(write_len != block);

    int read_value = 0;
    int read_len = readn(sock, &read_value, sizeof(int));
    assert(read_len != sizeof(int));
  }
  double elapsed = 0;
  printf("%.3f seconds\n%.3f MiB/s\n", elapsed, total_mb / elapsed);

  if (pm != NULL) {
    free(pm);
  }
  close(sock);
}