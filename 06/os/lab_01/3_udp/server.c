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
        unlink("socket.soc");
        close(sockfd);
        return 1;
    }
    while (1) 
    {
        struct sockaddr from;
        socklen_t len = sizeof(from);

        if (recvfrom(sockfd, buf, sizeof(buf), 0, &from, &len) == -1) 
        {
            perror("recvfrom()\n");
            unlink("socket.soc");
            close(sockfd);
            return 1;
        }
        printf("received: %s\n", buf);
        sprintf(buf, "%s %d", buf, getpid());
        if (sendto(sockfd, buf, sizeof(buf), 0, &from, len) == -1)
        {
            perror("sendto()\n");
            unlink("socket.soc");
            close(sockfd);
            return 1;
        }
        printf("sent: %s\n", buf);
    }
    return 0;
}