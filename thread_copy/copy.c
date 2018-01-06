/*************************************************************************
    > File Name: multi_thd_cp.c
    > Author: 
    > Mail: 
    > Created Time: 2017年12月23日 星期六 09时05分37秒
 ************************************************************************/

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#define MB (1024 * 1024 * 4)
typedef struct {
    int tfd;
    size_t size;
    size_t offset;
    char* buf;
} thread_info;
typedef struct {
    pthread_t* threads;
    size_t size;
    int thread_count;
    char* source;
    thread_info* info;
} task;
static inline void thread_info_init(thread_info* g, int tfd, size_t offset, size_t size, char* s_buf)
{
    g->size = size;
    g->offset = offset;
    g->buf = s_buf;
    //g->buf = (char*)calloc(1, size);
    //memcpy(g->buf, s_buf, size);
    g->tfd = tfd;
}
static task* task_create(const char* src, const char* dst, int thread_count)
{
    thread_count = thread_count <= 0 ? 1 : thread_count;
    int sfd = open(src, O_RDONLY);
    int dfd = open(dst, O_CREAT | O_RDWR);
    if (sfd == -1 || dfd == -1) {
        return NULL;
    }
    struct stat st;
    fstat(sfd, &st);
    task* t = (task*)calloc(sizeof(*t), 1);
    t->size = st.st_size;
    int64_t size = st.st_size / thread_count;
    int mode = st.st_size % thread_count;
    t->info = (thread_info*)calloc(sizeof(thread_info), thread_count);
    int64_t tmp = 0;
    char* s_buf = (char*)mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, sfd, 0);
    t->source = s_buf;
    t->thread_count = thread_count;
    t->threads = (pthread_t*)calloc(sizeof(pthread_t), thread_count);
    int rest_size = (mode == 0) ? size : size + mode;
    for (int i = 0; i < thread_count; i++) {
        int new_fd = open(dst, O_CREAT | O_RDWR);
        if (i == thread_count - 1) {
            thread_info_init(&t->info[i], new_fd, tmp, rest_size, s_buf + tmp);
            break;
        }
        thread_info_init(&t->info[i], new_fd, tmp, size, s_buf + tmp);
        tmp += size;
    }
    close(sfd);
    close(dfd);
    return t;
}
void* thread_run(void* arg)
{
    thread_info* g = (thread_info*)arg;
    int ret = 0;
    lseek(g->tfd, g->offset, SEEK_SET);
    if ((ret = write(g->tfd, g->buf, g->size)) != g->size) {
        fprintf(stdout, "write error:%s\n", strerror(errno));
    }
    fdatasync(g->tfd);
    return NULL;
}

void task_destroy(task* t)
{
    munmap(t->source, t->size);
    free(t->threads);
    free(t->info);
    free(t);
}
int main(int argc, char* argv[])
{
    if (argc != 4) {
        fprintf(stdout, "usage:%s -n=[thread_size] [src_file] [dst_file]\n", argv[0]);
        return -1;
    }
    char* p2 = strchr(argv[1], '=');
    int thd_size = atoi((++p2));
    task* t = task_create(argv[2], argv[3], thd_size);
    for (int i = 0; i < t->thread_count; i++) {
        pthread_create(&t->threads[i], NULL, (void*)&thread_run, (void*)(&t->info[i]));
    }
    for (int i = 0; i < t->thread_count; i++) {
        pthread_join(t->threads[i], NULL);
    }
    task_destroy(t);
    return 0;
}
