/*************************************************************************
	> File Name: multi_thd_cp.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月23日 星期六 09时05分37秒
 ************************************************************************/

#include <stdio.h>
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
#define MB (1024 * 1024)
#define PATH_MAX_SIZE 512
typedef struct _thd_arg {
    char source[PATH_MAX_SIZE];
    char target[PATH_MAX_SIZE];
    size_t start;
    size_t size;
} thd_arg;
static inline void thd_arg_init(thd_arg* g, const char* source, const char* target, size_t start, size_t size)
{
    memset(g->source, '\0', PATH_MAX_SIZE);
    memset(g->target, '\0', PATH_MAX_SIZE);
    strncpy(g->source, source, strlen(source));
    strncpy(g->target, target, strlen(target));
    g->start = start;
    g->size = size;
}
static thd_arg* thd_arg_create(const char* src, const char* dst, int thd_count)
{
    thd_count = thd_count <= 0 ? 1 : thd_count;
    int sfd = open(src, O_RDONLY);
    struct stat st;
    fstat(sfd, &st);
    int64_t per_size = st.st_size / thd_count;
    int rest_size = st.st_size % thd_count;
    thd_arg* tg = (thd_arg*)calloc(sizeof(*tg), thd_count);
    assert(tg != NULL);
    int64_t tmp = 0;
    rest_size = rest_size == 0 ? per_size : per_size + rest_size;
    for (int i = 0; i < thd_count; i++) {
        if (i == thd_count - 1 && rest_size != 0) {
            thd_arg_init(&tg[i], src, dst, tmp, rest_size);
            break;
        }
        thd_arg_init(&tg[i], src, dst, tmp, per_size);
        tmp += per_size;
    }
    int total_size = 0;
    for (int i = 0; i < thd_count; i++) {
        total_size += tg[i].size;
    }
    fprintf(stdout, "origin size=%d,now size=%d,threads =%d\n\n", st.st_size, total_size, thd_count);
    close(sfd);
    return tg;
    close(sfd);
}
void thd_copy(thd_arg* g)
{

    int rsize = g->size;

    int sfd = open((const char*)&g->source, O_RDONLY, 0644);
    int dfd = open((const char*)&g->target, O_CREAT | O_RDWR, 0644);
    fprintf(stdout, "thread # %ld,sfd=%d,dfd=%d,start_offset=%d,write_size=%d\n", pthread_self(), sfd, dfd, g->start, g->size);
    lseek(sfd, g->start, SEEK_SET);
    lseek(dfd, g->start, SEEK_SET);
    int wsize = 0;
    while (wsize <= rsize) {
        int rsize = 0;
        char buf[512] = { '\0' };
        if ((rsize = read(sfd, buf, sizeof(buf))) > 0) {
            //fprintf(stdout, "thread #%ld read :%s\n", pthread_self(), strerror(errno));
            if (write(dfd, buf, rsize) <= 0) {
                //fprintf(stdout, "thread #%ld write :%s\n", pthread_self(), strerror(errno));
                return;
            }
            wsize += rsize;
            rsize -= rsize;
            //fprintf(stdout, "thread #%ld write size=%d\n", pthread_self(), size);
        } else {
            break;
        }
    }
}
int main(int argc, char* argv[])
{
    if (argc != 4) {
        fprintf(stdout, "usage:%s -n=[thread_size] [src_file] [dst_file]\n", argv[0]);
        return -1;
    }
    char* p2 = strchr(argv[1], '=');
    int thd_size = atoi((++p2));

    pthread_t* thds = (pthread_t*)calloc(sizeof(pthread_t), thd_size);
    assert(thds != NULL);
    thd_arg* tg = thd_arg_create(argv[2], argv[3], thd_size);
    for (int i = 0; i < thd_size; i++) {
        pthread_create(&thds[i], NULL, (void*)&thd_copy, (void*)(&tg[i]));
    }
    for (int i = 0; i < thd_size; i++) {
        pthread_join(thds[i], NULL);
    }
    return 0;
}
