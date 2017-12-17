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
 */
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include  <unistd.h>
void *get_running_cpu(void *arg)
{
    
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF);
    
    cpu_set_t set;
    cpu_set_t get;
    CPU_ZERO(&set);
    CPU_ZERO(&get);
    CPU_SET(rand()%cpu_num,&set);
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
int main(void) {
    int size = 5;
    cpu_set_t mset,mget;
    CPU_ZERO(&mset);
    CPU_ZERO(&mget);
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF);
    CPU_SET(2,&mset);
    sched_setaffinity(getpid(),sizeof(cpu_set_t),&mset);
    sched_getaffinity(getpid(),sizeof(cpu_set_t),&mget);
    for(int i=0;i<cpu_num;i++)
    {
        if(CPU_ISSET(i,&mget)){
            fprintf(stdout,"main #%ld running processor %d\n\n",getpid(),i);
        }
    }
    pthread_t *threads = (pthread_t *)calloc(sizeof(pthread_t),size);
    for(int i=0;i<size;i++)
    {
        if(pthread_create(&threads[i],NULL,(void *)&get_running_cpu,(void *)NULL) == -1)
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
