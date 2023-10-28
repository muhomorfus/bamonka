#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define CHILD_NUM 2
int main() 
{
    pid_t child_pid[CHILD_NUM];
    for (int i = 0; i < CHILD_NUM; i++) 
    {
        if ((child_pid[i] = fork()) == -1) 
        {
            perror("fork() error\n");
            exit(1);
        }
        if (child_pid[i] == 0) 
        {   
            printf("child: pid = %d, ppid = %d, pgrp = %d (before sleep)\n", getpid(), getppid(), getpgrp());
            sleep(3);
            printf("child: pid = %d, ppid = %d, pgrp = %d (after sleep)\n", getpid(), getppid(), getpgrp());
            return 0;
        }
        printf("parent: pid = %d, pgrp = %d, child pid: %d\n", getpid(), getpgrp(), child_pid[i]);
    }
    return 0;
}
