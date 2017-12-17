/*************************************************************************
	> File Name: thread.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月17日 星期日 03时23分00秒
 ************************************************************************/

/*
 *
 *process : 
 *      1.owned pcb
 *      1.haved private address space 
 *thread:
 *      1.owned pcb indepently
 *      2.share memory and data with parent process
 *
 *     proessor1  -> pcb
 *                      |
 *                      一级页目录
 *                              |
 *                              二级页表                            虚拟地址空间
 *                                     |                            |
 *                                      物理内存   <---------->  mmu 
 *                                           |
 *                                            
 *     thread1   ->  pcb
 *                      |
 *                      share memory,fd resource with processor1
 *
 *
 *
 * process can reduce cpu cache miss by binding pid of main process to some cpu
 */
#define _GNU_SOURCE
#define THREADS_MAX 64
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include  <unistd.h>
void *get_running_cpu(void *arg)
{
    
    int *cpu = (int *)arg;
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF);

    cpu_set_t set;
    cpu_set_t get;
    CPU_ZERO(&set);
    CPU_ZERO(&get);
       
    if(*cpu > cpu_num) {
        CPU_SET(rand()%cpu_num,&set);
    }else{
        CPU_SET(*cpu,&set);
    }
    pthread_setaffinity_np(pthread_self(),sizeof(set),&set);
    //sched_setaffinity(0,sizeof(cpu_set_t),&set)   //first parameter is 0,can affect thread
    pthread_getaffinity_np(pthread_self(),sizeof(cpu_set_t),&get);
    for(int i=0;i<cpu_num;i++)
    {
        if(CPU_ISSET(i,&get)) {
            fprintf(stdout,"Thread #%ld running on cpu %d\n",pthread_self(),i);
        }
    }
    return NULL;
}
inline static bool is_digit(const char *s) {
    while(*s != '\0') {
        if(isdigit(*s++)==0)
        {
            return false;
        }
    }
    return true;
}
int main(int argc,char *argv[]) {
    int size = 0;
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF);
    if (argc !=4) {
        fprintf(stdout,"usage:./thread_test [main_cpu_id] [threads_size] [thread_bind_cpu]\n");
        fprintf(stdout,"                    --main_cpu_id   main processor\n");
        fprintf(stdout,"                    --threads_size  create thread size\n");
        fprintf(stdout,"                    --thread_bind_cpu  -1 using random cpu num,other value bind cpu num\n");
        return 0;
    }
    int main_cpu = 0;
    int threads_size = 0;
    int thread_cpu_num = 0;
    if(!is_digit(argv[1])) {
        main_cpu = cpu_num-1;
    }else{
        main_cpu = atoi(argv[1]) >cpu_num ?cpu_num-1:atoi(argv[1]);
        
    }
    if(!is_digit(argv[2])) {
        threads_size = (THREADS_MAX>>2);
    }else{
        threads_size = atoi(argv[2]);
    }
    if(!is_digit(argv[3])) {
        thread_cpu_num = rand()%cpu_num;
    }else{
        thread_cpu_num= atoi(argv[3]);
    }
    size = threads_size;
    pid_t pid = getpid();
    cpu_set_t mset,mget;
    CPU_ZERO(&mset);
    CPU_ZERO(&mget);
    CPU_SET(main_cpu,&mset);
    sched_setaffinity(pid,sizeof(cpu_set_t),&mset);
    sched_getaffinity(pid,sizeof(cpu_set_t),&mget);
    for(int i=0;i<cpu_num;i++)
    {
        if(CPU_ISSET(i,&mget)){
            fprintf(stdout,"Main #%ld running processor %d\n\n",getpid(),i);
        }
    }
    pthread_t *threads = (pthread_t *)calloc(sizeof(pthread_t),size);
    for(int i=0;i<size;i++)
    {
        if(pthread_create(&threads[i],NULL,(void *)&get_running_cpu,(void *)&thread_cpu_num) == -1)
        {
            fprintf(stdout,"%s\n",strerror(errno));
            return -1;
        }
    }
    for(int i=0;i<size;i++)
    {
        pthread_join(threads[i],NULL);
    }
    if (threads!=NULL) {
        free(threads);
    }
    return 0;
}
