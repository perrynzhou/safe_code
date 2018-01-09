/*************************************************************************
   > File Name: daemon.c
   > Author: perrynzhou
   > Mail: perrynzhou@gmail.com
   > Created Time: 2018年01月09日 星期二 08时51分57秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
/*
 *
 1.父进程执行fork,成功后父进程退出
 2.子进程中调用setsid函数，创建新的会话
 3.调用chdir函数，切换当前进程的工作目录
 4.调用umask函数设置子进程的掩码
 5.关闭所有的输入输出(stdin,stdout,stderr)
 *
 */
int run_daemon()
{
    pid_t pid = fork();

    if (pid == -1) {
        fprintf(stdout, "error:%s\n", strerror(errno));
        return -1;
    } else if (pid > 0) {
        fprintf(stdout, "parent %d process exit\n", pid);
        _exit(0);
    }
    umask(0);
    if (setsid() == -1) {
        fprintf(stdout, "setsid:%s\n", strerror(errno));
        _exit(0);
    } //确保不是首进程
    for (int i = 0; i < 3; i++) {
        close(i);
    }
    umask(0);
    pid = fork();

    chdir("/");

    if (pid != 0) {
        _exit(0);
    }
    fprintf(stdout, "daemon process %d\n", getpid());
    int stdfd = open("/dev/null", O_RDWR);
    dup2(stdfd, STDOUT_FILENO);
    dup2(stdfd, STDERR_FILENO);
    return 0;
}
int main(void)
{

    run_daemon();
    while (1) {
        sleep(1);
    }
    return 0;
}
