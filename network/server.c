/*************************************************************************
	> File Name: server.cc
	> Author: perrynzhou
	> Mail: perrynzhou@gmail.com
	> Created Time: 2017年12月25日 星期一 04时17分38秒
 ************************************************************************/
#include <stdio.h>
#include <arpa/inet.h>
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
#include <stdbool.h>
//thread per client mode
typedef struct {
    int m_read_fd;
    int m_conn_fd;
    int m_seconds;
    pthread_t m_thread;
} thread;
void thread_destroy(thread* thd);
static void* task(void* arg)
{
    thread* thd = (thread*)(arg);
    pthread_t thread_id = pthread_self();
    pthread_detach(thread_id);
    char buffer[64] = { '\0' };

    struct stat st;
    fstat(thd->m_read_fd, &st);

    char host[32] = { '\0' };
    struct sockaddr_in local;
    socklen_t len = sizeof(host);
    getpeername(thd->m_conn_fd, (struct sockaddr*)&local, &len);
    inet_ntop(AF_INET, &local.sin_addr, host, sizeof(host));

    printf("thread #%ld start,sleep %d seconds\n ", thread_id, thd->m_seconds);
    sleep(thd->m_seconds);
    int read_bytes = 0;
    bool status = true;
    while ((read_bytes = read(thd->m_read_fd, buffer, 64)) > 0) {
        if (send(thd->m_conn_fd, buffer, read_bytes, 0) == -1) {
            printf("send error:%ld\n", strerror(errno));
            status = false;
            break;
        }
    }
    /*
    *      *safe to close tcp:shutdown->recv->close
    *       *如果不call shutdown,send完毕后，直接close:
    *       * 1.在send之前或者之后，client有数据发送到服务端，会导致tcp buffer中的未发送到客户端的丢失(rst)
    *       *2.先调用shutdown,即使客户端在数据未收全前后发送数据，服务端关闭客户端链接都不会导致客户端的数据丢失
    *                         
    **/
    if (status) {
        shutdown(thd->m_conn_fd, 1); //safe to wirte complete
        while (recv(thd->m_conn_fd, buffer, 64, 0) > 0)
            ; //safe to write compelte
        printf("    ---thread #%ld transform %ld bytes success\n", thread_id, st.st_size);
    }
    thread_destroy(thd);
    return NULL;
}
thread* thread_create(int m_read_fd, int m_conn_fd, int m_seconds)
{
    thread* thd = (thread*)calloc(1, sizeof(*thd));
    assert(thd != NULL);
    thd->m_read_fd = m_read_fd;
    thd->m_conn_fd = m_conn_fd;
    thd->m_seconds = m_seconds;
    return thd;
}
void thread_destroy(thread* thd)
{
    if (thd->m_read_fd != -1) {
        close(thd->m_read_fd);
    }
    if (thd->m_conn_fd != -1) {
        close(thd->m_conn_fd);
    }
    free(thd);
    thd = NULL;
}
void thread_start(thread* thd)
{
    if (thd->m_read_fd != -1) {
        pthread_create(&thd->m_thread, NULL, &task, (void*)thd);
    }
}

int server_init_sock(int m_port, int m_listen)
{
    struct sockaddr_in servaddr;
    int m_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sock == -1) {
        printf("socket error:%s\n", strerror(errno));
        return -1;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(m_port);
    if (bind(m_sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        printf("bind  error:%s\n", strerror(errno));
        return -1;
    }
    if (listen(m_sock, m_listen) == -1) {
        printf("listen  error:%s\n", strerror(errno));
        return -1;
    }
    return m_sock;
}
void server_run(const char* path, int port, int listen, int seconds)
{
    if (access(path, F_OK) == -1) {
        printf("access error:%s\n", strerror(errno));
        return;
    }
    int m_sock = 0;
    if ((m_sock = server_init_sock(port, listen)) != -1) {
        printf("==============start tcp server  running  at %d port============ \n", port);
        while (true) {
            int conn_fd = 0;
            if ((conn_fd = accept(m_sock, (struct sockaddr*)NULL, NULL)) == -1) {
                printf("accept error:%s\n", strerror(errno));
                break;
            }
            thread* ct = thread_create(open(path, O_RDONLY, 0644), conn_fd, seconds);
            thread_start(ct);
        }
    }
}
int main(int argc, char* argv[])
{
    if (argc != 4) {
        fprintf(stdout, "usage:test [port] [target_file] [seconds]\n");
        return -1;
    }
    int port = atoi(argv[1]);
    int seconds = 0;
    if (argv[3] != NULL) {
        seconds = atoi(argv[3]);
    }
    server_run(argv[2], port, 1000, seconds);
    return 0;
}
