#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define CHILD_NUM 5
#define TEXT_LEN 50

int main(int argc, char ** argv)
{
    int fd[2];
    char buf[TEXT_LEN];
    pid_t child_pid[CHILD_NUM];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) < 0) 
    {
        perror("socketpair()\n");
        exit(1);
    }

    for (size_t i = 0; i < CHILD_NUM; i++) 
    {
        if ((child_pid[i] = fork()) == -1)
        {
            perror("fork()\n");
            exit(1);
        }
        if (child_pid[i] == 0)
        {
           // close(fd[1]);
            sprintf(buf, "%d", getpid());
            write(fd[0], buf, sizeof(buf));
            printf("child write: %d\n", getpid());
            read(fd[0], buf, sizeof(buf));
            printf("child %d read: %s\n", getpid(), buf);
            close(fd[0]);

            return EXIT_SUCCESS;
        }
        else 
        {
            //sleep(1);
            read(fd[1], buf, sizeof(buf));
            printf("parent read: %s\n", buf);
            sprintf(buf, "%s %d", buf, getpid());
            write(fd[1], buf, sizeof(buf));
            printf("parent write: %s\n", buf);
        }
    }

  //  close(fd[0]);
   // close(fd[1]);

    return EXIT_SUCCESS;
}
