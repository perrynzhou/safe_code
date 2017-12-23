/*************************************************************************
	> File Name: bufio.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月23日 星期六 02时29分44秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
const char *content =  "abcdefghijklmnopqrszuvwxyz\n";
void write_file(FILE *file,int loop,bool b)
{
    size_t len = strlen(content);
    int bsize=16384;
    char buf[16384]={'\0'};
    if(b) {
        if(setvbuf(file,buf,_IOLBF,bsize)==-1)
        {
            fprintf(stdout,"setvbuf line_buffer error:%s\n",strerror(errno));
            return;
        }
    }else{
        if(setvbuf(file,buf,_IONBF,bsize)==-1)
        {
            fprintf(stdout,"setvbuf no_buffer error:%s\n",strerror(errno));
            return;
        }
    }
    for(int i=0;i<loop;i++)
    {
        fwrite(content,len,1,file);
    }
}
int main(int argc,char *argv[])
{
    int loop = atoi(argv[2]);
    int flag = atoi(argv[1]);
    FILE *fp = fopen("./log","a+");
    assert(fp !=NULL);
    clock_t start=clock();
    if(flag == 0) {
        printf("no buffer io\n");
        write_file(fp,loop,false);
    }else{
        printf("buffer io\n");
        write_file(fp,loop,true);
    }
    fdatasync(fileno(fp));
    clock_t end = clock();
    double duration = (double)(end-start)/CLOCKS_PER_SEC;
    fprintf(stdout,"consumed time:%f seconds\n",duration);
    return 0;
}
