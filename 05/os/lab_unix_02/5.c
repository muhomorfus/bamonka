#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#define CHILD_NUM 2
int flag = 0;
void check_status(int status) 
{
    if (WIFEXITED(status)) 
        printf("child process has finished with code: %d\n\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status)) 
        printf("child process has finished with non-interceptable signal: %d\n\n", WTERMSIG(status));
    else if (WIFSTOPPED(status)) 
        printf("child process has stopped, signal: %d\n\n", WSTOPSIG(status));
}
void signal_handler(int signal) 
{
    flag = 1;
    printf("got signal %d\n", signal);
}
int main() 
{
    char text[CHILD_NUM][50] = {"aa aaa", "bbbbbbbbbbbbb ccc dmsdkcljk sd d ddmkdmkd"};
    int status;
    pid_t child_pid[CHILD_NUM];
    int fd[2];
    if (signal(SIGINT, signal_handler) == SIG_ERR) 
    {
        perror("signal() error\n");
        exit(1);
    }
    sleep(3);
    if (pipe(fd) == -1) 
    {
        perror("pipe() error\n");
        exit(1);
    }
    for (int i = 0; i < CHILD_NUM; i++) 
    {
        if ((child_pid[i] = fork()) == -1) 
        {
            perror("fork() error\n");
            exit(1);
        }
        if (child_pid[i] == 0) 
        {
            if (flag) 
            {
                close(fd[0]);
                write(fd[1], text[i], strlen(text[i]) + 1);
                printf("child: pid = %d, ppid = %d, pgrp = %d, message: %s\n", getpid(), getppid(), getpgrp(), text[i]);
            }
            return 0;
        }
    }
    for (int i = 0; i < CHILD_NUM; i++) 
    {
        text[i][0] = '\0';
        close(fd[1]);
        read(fd[0], text[i], strlen(text[i]) + 1);
        if (waitpid(child_pid[i], &status, WUNTRACED) == -1) 
        {
            perror("waitpid() error\n");
            exit(1);
        }
        printf("child: status = %d, pid = %d\n", status, child_pid[i]);
        check_status(status);
        printf("parent received from %d: %s\n", child_pid[i], text[i]);
    }
    return 0;
}