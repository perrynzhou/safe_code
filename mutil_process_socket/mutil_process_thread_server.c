/*************************************************************************
    > File Name: socket.c
  > Author: perrynzhou
  > Mail: perrynzhou@gmail.com 
  > Created Time: 2018年02月02日 星期五 00时24分33秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
static void* thread_process(void* arg)
{
    int* lid = (int*)arg;
    int listensock = *lid;
    int newsock = -1;
    char buffer[64] = { '\0' };
    while (1) {
        newsock = accept(listensock, NULL, NULL);
        if (newsock == -1) {
            break;
        } else {
            printf("client connected to thread %ld\n", pthread_self());
            int nread = recv(newsock, buffer, 64, 0);
            printf("%s\n", buffer);
            send(newsock, buffer, nread, 0);
            close(newsock);
            printf("client disconnected from thread  %ld\n", pthread_self());
            break;
        }
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
    int thread_size = 1;
    int x;
    int val;
    if (argc > 1) {
        nchildren = atoi(argv[1]);
    }
    if (argc > 2) {
        thread_size = atoi(argv[2]);
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
    for (x = 0; x < nchildren; x++) {

        if ((pid = fork()) == 0) {
            pthread_t* thds = (pthread_t*)calloc(sizeof(pthread_t), thread_size);
            int i = 0;
            for (; i < thread_size; i++) {
                pthread_create(&thds[i], NULL, &thread_process, (void*)&listensock);
            }
            sched_yield();
            for (i = 0; i < thread_size; i++) {
                pthread_join(thds[i], NULL);
            }
            if (thds != NULL) {
                free(thds);
            }
        } else {
            if (x == 0) {
                fprintf(stdout, "-------main pid=%ld,create %d process,each process contains %d thread for client-----------\n", getpid(), nchildren, thread_size);
                fprintf(stdout, "-----------listen port %d--------\n", 1972);
            }
        }
    }
    wait(NULL);
}
