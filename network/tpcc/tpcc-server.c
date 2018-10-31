/*************************************************************************
  > File Name: tpcc-server.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: 三 10/31 10:15:53 2018
 ************************************************************************/

#include "tpcc.h"
#include "dict.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define BACKLOG (1024)
#define MAX_BUCKET_SIZE (16383)
static pthread_mutex_t lock;
static dict *map;

typedef struct request
{
  session_msg sm;
  int cfd;
  pthread_t id;
  bool is_first;
} request;
static request *request_create(int cfd)
{
  request *req = (request *)calloc(1, sizeof(*req));
  req->is_first = false;
  req->cfd = cfd;
}
static int client_cmp(const void *key1, const void *key2)
{
  int *ptr1 = (int *)key1;
  int *ptr2 = (int *)key2;
  if (*ptr1 == *ptr2)
  {
    return 0;
  }
  else if (*ptr1 > *ptr2)
  {
    return -1;
  }
  else
  {
    return 1;
  }
}
size_t client_len(const void *key)
{
  return sizeof(int);
}
static int init_socket(int port)
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    printf("socket error: %s(errno: %d)\n", strerror(errno), errno);
    return -1;
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);

  if (bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0)
  {
    printf("bind  error: %s(errno: %d)\n", strerror(errno), errno);
    return -1;
  }
  if (listen(sock, BACKLOG) < 0)
  {
    printf("listen error: %s(errno: %d)\n", strerror(errno), errno);
    return -1;
  }
  return sock;
}
void handle_connection(void *arg)
{

  request *req = (request *)arg;
  if (recv(req->cfd, &req->sm, sizeof(session_msg), MSG_NOSIGNAL) == -1)
  {
    printf("send error: %s(errno: %d)\n", strerror(errno), errno);
  }
  fprintf(stdout, "server recv session_msg:number=%d,length=%d\n", req->sm.number, req->sm.length);
  payload_msg pm;
  pm.length = req->sm.length + 4;
  for (int i = 0; i < req->sm.number; i++)
  {
    if (recv(req->cfd, &pm, sizeof(pm), 0) == -1)
    {
      printf("recv error: %s(errno: %d)\n", strerror(errno), errno);

    } //block function
    int v = rand() % 1024;
    if (send(req->cfd, &v, sizeof(int), MSG_NOSIGNAL) == -1)
    {
      printf("send error: %s(errno: %d)\n", strerror(errno), errno);
      continue;
    }
    char sock_info[128] = {'\0'};
    get_sock_info(req->cfd, (char *)&sock_info);
    fprintf(stdout, "::client thread %ld write to %s\n", pthread_self(), sock_info);
  }
  int cfd_dup = req->cfd;
  shutdown(req->cfd, SHUT_WR);
  close(req->cfd);
  dict_del(map, &cfd_dup);
}
void handle_accept_request(void *arg)
{
  int *sock = (int *)arg;
  fprintf(stdout, "******start socket service %ld******\n", pthread_self());
  while (1)
  {
    int cfd = accept(*sock, (struct sockaddr *)NULL, NULL);
    if (cfd == -1)
    {
      continue;
    }
    request *req = (request *)dict_fetch(map, &cfd);
    if (req == NULL)
    {
      req = request_create(cfd);
      pthread_mutex_lock(&lock);
      dict_add(map, &req->cfd, req, 0);
      req->is_first = true;
      pthread_mutex_unlock(&lock);
      pthread_create(&req->id, NULL, (void *)&handle_connection, (void *)req);
    }
  }
}

int main(int argc, char *argv[])
{
  if(argv[1]  !=NULL && strncmp(argv[1],"-h",2)==0) {
        fprintf(stdout,"\nusage:%s {port} {thread_count}\n",argv[0]);
        return -1;
  }
  int port = (NULL == argv[1]) ? 6789 : atoi(argv[1]);
  int sock = init_socket(port);
  if (sock == -1)
  {
    return -1;
  }
  int thd_size = (NULL == argv[2]) ? 1 : atoi(argv[2]);
  if (thd_size <= 0)
  {
    thd_size = 1;
  }
  signal(SIGPIPE, SIG_IGN);
  pthread_mutex_init(&lock, NULL);
  if (map == NULL)
  {
    map = dict_create(NULL, MAX_BUCKET_SIZE, 0);
    map->key_cmp = &client_cmp;
    map->key_len = &client_len;
    map->key_destroy = map->val_destroy = &free;
  }
  pthread_t thds[thd_size];
  fprintf(stdout, "-------server start at %d------\n", port);
  for (int i = 0; i < thd_size; i++)
  {
    pthread_create(&thds[i], NULL, (void *)&handle_accept_request, (void *)&sock);
  }
  for (int i = 0; i < thd_size; i++)
  {
    pthread_join(thds[i], NULL);
  }
  if (sock != -1)
  {
    close(sock);
  }
  dict_destroy(map);
  fprintf(stdout, "::server stop at %d::\n", port);
}