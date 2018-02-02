/*************************************************************************
    > File Name: socket.c
  > Author: perrynzhou
  > Mail: perrynzhou@gmail.com 
  > Created Time: 2018年02月02日 星期五 00时24分33秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
void exit_func(int sig)
{
    if (sig == SIGINT) {
        fprintf(stdout, "processor %ld stop!", getpid());
        _exit(0);
    }
}
int main(int argc, char* argv[])
{
    struct sockaddr_in sAddr;
    int listensock;
    int newsock;
    char buffer[25];
    int result;
    int nread;
    int pid;
    int nchildren = 1;
    int x;
    int val;
    if (argc > 1) {
        nchildren = atoi(argv[1]);
    }
    listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    val = 1;
    result = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (result < 0) {
        perror("server3");
        return 0;
    }
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(1972);
    sAddr.sin_addr.s_addr = INADDR_ANY;
    result = bind(listensock, (struct sockaddr*)&sAddr, sizeof(sAddr));
    if (result < 0) {
        perror("server3");
        return 0;
    }
    result = listen(listensock, 5);
    if (result < 0) {
        perror("server3");
        return 0;
    }
    signal(SIGINT, exit_func);
    for (x = 0; x < nchildren; x++) {
        if ((pid = fork()) == 0) {
            while (1) {
                newsock = accept(listensock, NULL, NULL);
                printf("client connected to child process %i.\n", getpid());
                nread = recv(newsock, buffer, 64, 0);
                buffer[nread] = '\0';
                printf("%s\n", buffer);
                send(newsock, buffer, nread, 0);
                close(newsock);
                printf("client disconnected from child process %i.\n", getpid());
            }
        } else {
            if (x == 0) {
                fprintf(stdout, "-------main pid=%ld,create %d process for clients-----------\n", getpid(), nchildren);
                fprintf(stdout, "-----------listen port %d--------\n", 1972);
            }
        }
    }
    wait(NULL);
}
