#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#define CHILD_NUM 2
void check_status(int status) 
{
    if (WIFEXITED(status)) 
        printf("child process has finished with code: %d\n\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status)) 
        printf("child process has finished with non-interceptable signal: %d\n\n", WTERMSIG(status));
    else if (WIFSTOPPED(status)) 
        printf("child process has stopped, signal: %d\n\n", WSTOPSIG(status));
}
int main() 
{
    pid_t child_pid[CHILD_NUM];
    int status;
    for (int i = 0; i < CHILD_NUM; i++) 
    {
        if ((child_pid[i] = fork()) == -1) 
        {
            perror("fork() error\n");
            exit(1);
        }
        if (child_pid[i] == 0) 
        {   
            printf("child: pid = %d, ppid = %d, pgrp = %d\n", getpid(), getppid(), getpgrp());
            return 0;
        }
        printf("parent: pid = %d, pgrp = %d, child pid: %d\n", getpid(), getpgrp(), child_pid[i]);
    }
    for (int i = 0; i < CHILD_NUM; i++) 
    {
        if (waitpid(child_pid[i], &status, WUNTRACED) == -1) 
        {
            perror("waitpid() error\n");
            exit(1);
        }
        printf("\nchild: pid = %d, status = %d\n", child_pid[i], status);
        check_status(status);
    }
    return 0;
}
