/*************************************************************************
	> File Name: srv.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月25日 星期一 04时17分38秒
 ************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <assert.h>
#include <fcntl.h>
typedef struct _client {
    pthread_t t;
    int file_fd;
    int cli_fd;
}client;
int server_init(int port)
{
    int fd;  
    struct sockaddr_in     servaddr;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        printf("%s\n",strerror(errno));
        return -1;
    }
     memset(&servaddr, 0, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
     servaddr.sin_port = htons(port);
    if(bind(fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1 ){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return -1;    
    }
    if(listen(fd, 10) == -1 ){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return -1;
    }
    return fd;

}
static client *client_init(const char *path,int cli_fd)
{
    client *c=(client *)calloc(1,sizeof(*c));
    assert(c!=NULL);
    c->file_fd=open(path,O_RDONLY,0644);
    c->cli_fd =cli_fd;
    return c;
}
static void *send_file(void *arg)
{
    client *c = (client *)arg;
    pthread_detach(pthread_self());
    int file_fd = c->file_fd;
    int cli_fd = c->cli_fd;
    char r_buf[64] = {'\0'};
    struct stat st;
    fstat(c->file_fd,&st);
    int r_size=0;
    int second=10;

    char ip[32]={'\0'};
    struct sockaddr_in local;
    socklen_t len= sizeof(ip);
    getpeername(c->cli_fd, (struct sockaddr *)&local, &len);
    inet_ntop(AF_INET, &local.sin_addr, ip, sizeof(ip));
    fprintf(stdout,"server accept  client %s:%d   [thread #%ld sleep %d]\n",ip,ntohs(local.sin_port),pthread_self(),second);
    sleep(second);
    while((r_size=read(c->file_fd,r_buf,64))>0)
    {
        if(send(c->cli_fd,r_buf,r_size,0)==-1)
        {
            fprintf(stdout,"send error:%s\n",strerror(errno));
            break;
        }
    }
    /*
     *safe to close tcp:shutdown->recv->close
     *如果不call shutdown,send完毕后，直接close:
    * 1.在send之前或者之后，client有数据发送到服务端，会导致tcp buffer中的未发送到客户端的丢失(rst)
     *2.先调用shutdown,即使客户端在数据未收全前后发送数据，服务端关闭客户端链接都不会导致客户端的数据丢失
     *
     */
    shutdown(c->cli_fd,1);
    while(recv(c->cli_fd,r_buf,64,0)>0);
    if(c!=NULL)
    {
        close(c->cli_fd);
        close(c->file_fd);
        free(c);
    }
    fprintf(stdout,"thread #%ld send  %ld bytes success\n",pthread_self(),st.st_size); 
    return NULL;
}
void server_run(const char *file,int fd)
{
    int file_fd = open(file,O_RDONLY);
    if(file_fd == -1) {
        printf("open file error:%s\n",strerror(errno));
        return;
    }
    close(file_fd);
    while(true){
        int connfd=0;
        if( (connfd = accept(fd, (struct sockaddr*)NULL, NULL)) == -1 ){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            break;
        }
        client *c=client_init(file,connfd);
        pthread_create(&c->t,NULL,(void *)&send_file,(void *)c);
    }
}
int main(int argc,char *argv[])
{
    int port = atoi(argv[1]);
    int fd = server_init(port);
    server_run(argv[2],fd);
    if(fd !=-1)
    {
        close(fd);
    }
    return 0;
}
