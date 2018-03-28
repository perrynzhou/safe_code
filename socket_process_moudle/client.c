/*************************************************************************
    > File Name: client.c
  > Author: perrynzhou
  > Mail: perrynzhou@gmail.com
  > Created Time: 2018年03月25日 星期日 22时51分44秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
static void *client_create(void *arg) {
    int *p_port = (int *)arg;
    if(p_port==NULL) {
        return NULL;
    }
    //fprintf(stdout,"....start client #%ld...\n",pthread_self());
    int port = *p_port;
    int sock;
    int buffer_size=1024;
    struct sockaddr_in cli_addr;
    char buffer[buffer_size];
    memset((void *)&cli_addr,0,sizeof(cli_addr));

    cli_addr.sin_family=AF_INET;
    cli_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    cli_addr.sin_port = htons(port);

    pthread_t self = pthread_self();
    sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock == -1) {
        printf("socket :%s\n",strerror(errno));
        goto _CLOSE;
    }
    if(connect(sock,(struct sockaddr *)&cli_addr,sizeof(cli_addr))==-1)
    {
        printf("connect:%s\n",strerror(errno));
        goto _CLOSE;
    }
    sprintf((char *)&buffer,"i am client %ld",self);
    if(send(sock,&buffer,strlen(buffer),0) ==-1)
    {
        printf("send:%s\n",strerror(errno));
        goto _CLOSE;
    }
    memset(&buffer,'\0',buffer_size);
    int nread = 0;
    if((nread=recv(sock,&buffer,buffer_size,0)) > 0){
        buffer[nread] = '\0';
        printf("client recieve message:%s",buffer);
    }
    close(sock);
    return NULL;
_CLOSE:
    if(sock!=-1) {
        close(sock);
    }
    return NULL;
}
void usage(char *pro) {
    fprintf(stdout,"usage:%s {port} {thread_size} {running_time_seconds}\n",pro);
    fprintf(stdout,"      --port  listen server port\n");
    fprintf(stdout,"      --thead_size size of client per thread\n");
    fprintf(stdout,"      --running_time_seconds  how long seconds time for running\n");
}
bool check(char *s) {
    if(s==NULL) {
        return false;
    }
    while(*s!='\0') {
        if(isdigit(*s++)==0){
            return false;
        }
    }
    return true;
}
int main(int argc,char *argv[])
{
    if(argc !=4) {
        usage(argv[0]);
        return -1;
    }
    if (!check(argv[1]) || !check(argv[2]) || !check(argv[3])) {
        usage(argv[0]);
        return -1;
    }
    int port = atoi(argv[1]);
    int client_size = atoi(argv[2]);
    pthread_t thds[client_size];
    int n = atoi(argv[3]);
    while(n>=0) {
        for(int i=0;i<client_size;i++)
         {
            pthread_create(&thds[i],NULL,(void *)&client_create,(void *)&port);    
        }
        for(int i=0;i<client_size;i++){
            pthread_join(thds[i],NULL);
         }
        sleep(1);
        __sync_sub_and_fetch(&n,1);
    }
    return 0;
}

