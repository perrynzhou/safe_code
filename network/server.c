/*************************************************************************
	> File Name: srv.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月25日 星期一 04时17分38秒
 ************************************************************************/
#include <stdbool.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
int srv_init(int port)
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
void srv_run(const char *file,int fd)
{
    FILE *fp = fopen(file,"r");
    if(fp == NULL) {
        printf("open file error:%s\n",strerror(errno));
        return;
    }
    while(true){
        int connfd;
        if( (connfd = accept(fd, (struct sockaddr*)NULL, NULL)) == -1 ){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            break;
        }
        printf("sleep 10 seconds, after that transport file\n");
        sleep(10);
        char rbuf[64] = {'\0'};
        if(fgets(rbuf,64,fp)!=NULL)
        {
            if(send(connfd,rbuf,64,MSG_NOSIGNAL) == -1)
            {
                printf("send error:%s\n",strerror(errno));
                break;
            }
        }
        printf("send file success\n");
        fclose(fp);
        char wbuf[64] = {'\0'};
        if(recv(connfd,wbuf,64,0) == -1)
        {
            printf("recv error:%s\n",strerror(errno));
            break;
        }
        printf("recv msg:%s\n",wbuf);
    }
}
int main(int argc,char *argv[])
{
    int port = atoi(argv[1]);
    int fd = srv_init(port);
    srv_run(argv[2],fd);
    if(fd !=-1)
    {
        close(fd);
    }
    return 0;
}
