/*************************************************************************
    > File Name: process_per_socket.c
  > Author: perrynzhou
  > Mail: perrynzhou@gmail.com
  > Created Time: 2018年03月25日 星期日 22时02分53秒
 ************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#define MAX_BUF_SIZE 1024
const char *send_msg_format = " thread #%ld from %ld meet you\n";
typedef struct request_s {
    int sock;
    char in[MAX_BUF_SIZE];
    char out[MAX_BUF_SIZE];
    pid_t pt;//parent process id
    pthread_t ct; //client thread
}request_t;
request_t *request_new(int sock,pid_t pt) {
    request_t *req = (request_t *)malloc(sizeof(*req));
    assert(req);
    req->sock = sock;
    req->pt = pt;
    memset(&req->in,'\0',MAX_BUF_SIZE);
    memset(&req->out,'\0',MAX_BUF_SIZE);
    return req;
}
void *request_handle(void *arg) {
    request_t *req = (request_t *)arg;
    if(req==NULL || req->sock==-1) {
        goto _FAILED;
    }
    pthread_t self=pthread_self();
    ssize_t rsize = 0;
    if((rsize=recv(req->sock,&req->in,MAX_BUF_SIZE,0)) ==-1) {
        fprintf(stdout,"recv:%s\n",strerror(errno));
        goto _FAILED;
    }
    fprintf(stdout," child client thread %ld from  process %ld\n",self,req->pt);
    fprintf(stdout,"   |->%ld recv:%s\n",self,req->in);
    sprintf((char *)&req->out,send_msg_format,self,req->pt);
    if(send(req->sock,&req->out,MAX_BUF_SIZE,0)==-1) {
        goto _FAILED;
    }
    close(req->sock);
    free(req);
    req=NULL;
_FAILED:
    if(req!=NULL) {
        if(req->sock!=-1) {
            close(req->sock);
        }
        free(req);
        req=NULL;
    }
    return NULL;
}
int init_socket_listen(int port)
{
    int max_backlog =1024;
    struct sockaddr_in srv_addr;
    int val =1;
    int listen_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) ==-1)
    {
        fprintf(stdout,"setsocketopt:%s\n",strerror(errno));
        return -1;
    }

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listen_sock,(struct sockaddr *)&srv_addr,sizeof(srv_addr)) ==-1)
    {
        fprintf(stdout,"bind:%s\n",strerror(errno));
        return -1;
    }

    if(listen(listen_sock,max_backlog)==-1)
    {
        fprintf(stdout,"listen:%s\n",strerror(errno));
        return -1;
    }
    return listen_sock;
}
void usage(char *pro) {
    fprintf(stdout,"usage:pro {port} {thread_size}\n");
    fprintf(stdout,"          --port   server listen port\n");
    fprintf(stdout,"          --process_size   process for dispatch server\n");
}
int main(int argc,char *argv[])
{
    if(argc != 3) {
        usage(argv[0]);
        return -1;
    }
    int port = atoi(argv[1]);
    int n = atoi(argv[2]);
    int listen = init_socket_listen(port);
    if(listen == -1) {
        fprintf(stdout,"listen is %d\n",listen);
        return -1;
    }
	pid_t pid;
    int sock = 0;
    for(int i=0;i<n;i++) {
        if((pid=fork())==0)  {
            pid_t cur_pid = getpid();
            fprintf(stdout,"...start process %ld provide service...\n",cur_pid);
            while(1) {
                sock = accept(listen,NULL,NULL);
                if(sock == -1) {
                    continue;
                }
                //fprintf(stdout,"#%ld process accept a connection\n",cur_pid);
                request_t *req=request_new(sock,cur_pid);
                pthread_create(&req->ct,NULL,(void *)request_handle,(void *)req);
                pthread_join(req->ct,NULL);
            }
	    }
    } 
    wait(NULL);
    return 0;
}
