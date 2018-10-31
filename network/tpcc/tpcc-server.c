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
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define BACKLOG (1024)
#define MAX_BUCKET_SIZE (16383)
static pthread_mutex_t  lock;
static dict      *map;

typedef struct request {
  session_msg sm;
  bool is_first;
}request;
inline static request *equest_create() {
  request *req = (request *)calloc(1,sizeof(*req));
  req->is_first = false;
}
static void request_destroy(request *req) {
  if(req !=NULL) {
    free(req);
    req = NULL;
  }
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
static int init_socket(int port) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    perror("socket happen error");
    return -1;
  }
  if (listen(sock, BACKLOG) < 0) {
    perror("listen happen error");
    return -1;
  }
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);

  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind happen error");
    return -1;
  }
  return sock;
}
void thread_cb(void *arg) {
  int *sock = (int *)arg;
  pthread_t self = thread_self();
  fprintf(stdout,"::start worker %ld",self);
  while (1) {
    int cfd = accept(*sock, (struct sockaddr *)NULL, NULL);
    if (cfd == -1) {
      continue;
    }
    request *req = (request *)dict_fetch(map,&cfd);
    if(req== NULL) {
      req = request_create();
      read(cfd,req->sm,sizeof(session_msg));
      int *key = (int *)malloc(sizeof(int));
      *key = cfd;
      pthread_mutex_lock(&lock);
      dict_add(map,key,req,0);
      req->is_first= true;
      pthread_mutex_unlock(lock);
    }
    payload_msg pm;
    if(read(cfd,&pm,sizeof(pm)) ==-1) {
      perror("##read happen error");
      continue;
    }
    pm.length = req->sm.length+4;
    if(write(cfd,&pm.length,sizeof(int32_t)) ==-1) {
        perror("##write happen error");
        continue;
    }
    char sock_info[128] = {'\0'};
    get_sock_info(cfd,&sock_info);
    fprintf(stdout,"::worker %ld write to %s",self,sock_info);
  }
  pthread_detach(self);
}
int main(int argc, char *argv[]) {
  if (argc != 3) {
    return usage(argv[0]);
  }

  int port = (NULL == argv[1])?6789:atoi(argv[1]);
  int sock = init_socket(port);
  int thd_size  = (NULL == argv[2])?1:atoi(argv[2]);

  pthread_mutex_init(&lock,NULL);
  if(map == NULL) {
    map = dict_create(NULL,MAX_BUCKET_SIZE,0);
    map->key_cmp = &client_cmp;
    map->key_len = &client_len;
    map->key_destroy = &free;
    map->val_destroy = &request_destroy;

  }
  pthread_t  thds[thd_size];
  fprintf(stdout, "::server start at %d::\n", port);
  for(int i=0;i<thd_size;i++) {
    pthread_create(&thds[i],NULL,(void *)&thread_cb,(void *)&sock);
  }
  if(sock != -1) {
    close(sock);
  }
  dict_destroy(map);
  fprintf(stdout, "::server stop at %d::\n", port);
}