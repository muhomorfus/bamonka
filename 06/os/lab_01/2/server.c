#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 50

int sockfd;

void signal_handler(int signal)
{
    unlink("socket.soc");
    close(sockfd);
    printf("exiting...\n");
    exit(0);
}

int main(int argc, char **argv)
{
    char buf[BUF_SIZE];
    if (signal(SIGINT, signal_handler) == SIG_ERR)
    {
        perror("signal()\n");
        return 1;
    }
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("socket()\n");
        return 1;
    }
    struct sockaddr sa;
    sa.sa_family = AF_UNIX;
    strcpy(sa.sa_data, "socket.soc");
    if (bind(sockfd, &sa, sizeof(sa)) == -1) 
    {
        perror("bind()\n");
        return 1;
    }
    while (1) 
    {
        if (recvfrom(sockfd, buf, sizeof(buf), 0, NULL, NULL) == -1) 
        {
            perror("recvfrom()\n");
            unlink("socket.soc");
            close(sockfd);
            return 1;
        }
        printf("received: %s\n", buf);
    }
    return 0;
}