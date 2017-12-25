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
#define MB  (1024*1024)
typedef struct _thd_arg {
    int sfd;
    int dfd;
    size_t start;
    size_t size;
}thd_arg;
static inline void thd_arg_init(thd_arg *g,int src,int dst,size_t start,size_t size)
{
    g->sfd=src;
    g->dfd=dst;
    g->start=start;
    g->size=size;
}
static thd_arg *thd_arg_create(const char *src,const char *dst,int thd_count)
{
    if(src == NULL || dst==NULL)
    {
        goto _FAILED;
    }
    thd_count = thd_count <=0?1:thd_count;
    int sfd = open(src,O_RDONLY);
    int dfd = open(dst,O_CREAT|O_RDWR,0644);
    if(sfd == -1|| dfd == -1) {
        fprintf(stdout,"%s\n",strerror(errno));
        goto _FAILED;
    }
    struct stat st;
    fstat(sfd,&st);
    int64_t per_size = st.st_size/thd_count;
    int  rest_size = st.st_size%thd_count;
    thd_arg *tg=(thd_arg *)calloc(sizeof(*tg),thd_count);
    assert(tg!=NULL);
    int64_t tmp=0;
    for(int i=0;i<thd_count;i++)
    {
        if(i==thd_count-1 && rest_size!=0) {
            thd_arg_init(&tg[i],sfd,dfd,tmp,rest_size);
            break;
        }
        thd_arg_init(&tg[i],sfd,dfd,tmp,per_size);
        tmp+= per_size;
    }
    int total_size = 0;
    for(int i=0;i<thd_count;i++)
    {
        total_size+=tg[i].size;
    }
    fprintf(stdout,"origin size=%d,now size=%d,threads =%d\n",st.st_size,total_size,thd_count);
    /*
     *
     *
     */
    return tg;
_FAILED:
    return NULL;
}
void thd_copy(thd_arg *g)
{
    
    int   rsize= g->size;
    char buf[MB] = {'\0'};
    lseek(g->sfd,g->start,SEEK_SET);
    lseek(g->dfd,g->start,SEEK_SET);
    int  wsize = 0;
    while(wsize <= rsize)
    {
            int size = 0;    
        if((size = read(g->sfd,buf,sizeof(buf)))== -1)
            {
                fprintf(stdout,"thread #%ld read :%s\n",pthread_self(),strerror(errno));
                return;
            }
            if(write(g->dfd,buf,size) ==-1)
            {
                fprintf(stdout,"thread #%ld write :%s\n",pthread_self(),strerror(errno));
                return;
            }
            wsize+=size;
            rsize-=size;
        if(size == 0){
            break;
        }
            fprintf(stdout,"thread #%ld write size=%d\n",pthread_self(),size);
    }
}
int main(int argc,char *argv[])
{
    if(argc!=4){
        fprintf(stdout,"usage:%s -n=[thread_size] [src_file] [dst_file]\n",argv[0]);
        return -1;
    }
    char *p2 = strchr(argv[1],'=');
    int thd_size =atoi((++p2));
    
    pthread_t *thds =(pthread_t *)calloc(sizeof(pthread_t),thd_size);
    assert(thds!=NULL);
    thd_arg *tg = thd_arg_create(argv[2],argv[3],thd_size);
    for(int i=0;i<thd_size;i++) 
    {
        pthread_create(&thds[i],NULL,(void *)&thd_copy,(void *)(&tg[i]));
    }
    for(int i=0;i<thd_size;i++)
    {
        pthread_join(thds[i],NULL);
    }
    return 0;
}
