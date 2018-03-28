/*************************************************************************
    > File Name: process_per_socket.c
  > Author: perrynzhou
  > Mail: perrynzhou@gmail.com
  > Created Time: 2018年03月25日 星期日 22时02分53秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
void signal_handler(int signo) {
    while(waitpid(-1,NULL,WNOHANG)>0);
    fprintf(stdout,"...call signal handler...\n");
}
int main(int argc,char *argv[])
{
    int buf_size=1024;
    int max_backlog =1024;
    struct sockaddr_in srv_addr;
    int listen_sock,sock;
    char buffer[buf_size];
    int nread,val;
    pid_t pid;
    listen_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    val = 1;
    if(setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) ==-1)
    {
        fprintf(stdout,"setsocketopt:%s\n",strerror(errno));
        return -1;
    }

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(atoi(argv[1]));
    srv_addr.sin_addr.s_addr = INADDR_ANY;

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
    signal(SIGCHLD,signal_handler);
    fprintf(stdout,"server running at %s\n",argv[1]);
    while(1) {
        sock = accept(listen_sock,NULL,NULL);
        if((pid=fork())==0) {
            int child = getpid();
            fprintf(stdout,"child process %i created...\n",child);
            close(listen_sock);
            memset(&buffer,'\0',1024);
            if((nread = recv(sock,buffer,1024,0)) ==-1) {
                //fprintf(stdout,"recv :%s\n",strerror(errno));
                close(sock);
                exit(0);
            }
            buffer[nread] = '\0';
            fprintf(stdout,"child %d recv msg:%s\n",child,buffer);
            memset(buffer,'\0',1024);
            sprintf((char *)&buffer,"hello server,%d already recv message\n",child);
            if(send(sock,buffer,strlen(buffer),MSG_DONTWAIT)==-1) {
                fprintf(stdout,"send:%s\n",strerror(errno));
                close(sock);
                exit(0);
            }
            fprintf(stdout,"child %d send message ok!\n",child);
            close(sock);
            exit(0);
        }
        close(sock);
    }
}
