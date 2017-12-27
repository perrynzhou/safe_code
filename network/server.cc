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
#include <new>
#include <iostream>
static void* Task(void* arg);
class CThread {
public:
    CThread(int read_fd, int conn_fd)
        : m_read_fd(read_fd)
        , m_conn_fd(conn_fd)
    {
    }
    ~CThread();
    void Start();
    int m_read_fd;
    int m_conn_fd;

private:
    pthread_t m_thread;
};
class Server {
public:
    Server(int port, int listen)
        : m_port(port)
        , m_listen(listen)
    {
    }
    void Run(const char* path);

private:
    int m_port;
    int m_sock;
    int m_listen;
    bool initSock();
};

static void* Task(void* arg)
{
    CThread* cthread = static_cast<CThread*>(arg);
    pthread_t thread_id = pthread_self();
    pthread_detach(thread_id);
    char buffer[64] = { '\0' };

    struct stat st;
    fstat(cthread->m_read_fd, &st);

    char host[32] = { '\0' };
    struct sockaddr_in local;
    socklen_t len = sizeof(host);
    getpeername(cthread->m_conn_fd, (struct sockaddr*)&local, &len);
    inet_ntop(AF_INET, &local.sin_addr, host, sizeof(host));

    int seconds = 10;
    std::cout << "start thread #" << thread_id << ",that will sleep " << seconds << std::endl;
    //sleep(seconds);
    int read_bytes = 0;
    bool status = true;
    while ((read_bytes = read(cthread->m_read_fd, buffer, 64)) > 0) {
        if (send(cthread->m_conn_fd, buffer, read_bytes, 0) == -1) {
            std::cout << "send error:" << strerror(errno) << std::endl;
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
        shutdown(cthread->m_conn_fd, 1); //safe to wirte complete
        while (recv(cthread->m_conn_fd, buffer, 64, 0) > 0)
            ; //safe to write compelte
        std::cout << "thread #" << thread_id << " send  " << st.st_size << " bytes success" << std::endl;
    }
    delete cthread;
    return NULL;
}
CThread::~CThread()
{
    if (m_read_fd != -1) {
        close(m_read_fd);
    }
    if (m_conn_fd != -1) {
        close(m_conn_fd);
    }
}
void CThread::Start()
{
    if (m_read_fd != -1) {
        pthread_create(&m_thread, NULL, &Task, (void*)this);
    }
}
bool Server::initSock()
{
    struct sockaddr_in servaddr;
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sock == -1) {
        std::cout << "socket error:" << strerror(errno) << std::endl;
        return false;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(m_port);
    if (bind(m_sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        std::cout << "bind  error:" << strerror(errno) << std::endl;
        return false;
    }
    if (listen(m_sock, m_listen) == -1) {
        std::cout << "listen  error:" << strerror(errno) << std::endl;
        return false;
    }
    return true;
}
void Server::Run(const char* path)
{
    if (access(path, F_OK) == -1) {
        std::cout << "access error:" << strerror(errno) << std::endl;
        return;
    }
    if (initSock()) {
        std::cout << "==============start tcp server " << m_sock << " at " << m_port << "================" << std::endl;
        while (true) {
            int conn_fd = 0;
            if ((conn_fd = accept(m_sock, (struct sockaddr*)NULL, NULL)) == -1) {
                std::cout << "accept error:" << strerror(errno) << std::endl;
                break;
            }
            CThread* ct = new CThread(open(path, O_RDONLY, 0644), conn_fd);
            ct->Start();
        }
    }
}
int main(int argc, char* argv[])
{
    int port = atoi(argv[1]);
    Server server(port, 1000);
    server.Run(argv[2]);
    return 0;
}
