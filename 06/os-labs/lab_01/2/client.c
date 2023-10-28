#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>

#define BUF_SIZE 50

int main(int argc, char **argv)
{
    char buf[BUF_SIZE];
    sprintf(buf, "%d", getpid());
    int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("socket()\n");
        return 1;
    }
    struct sockaddr sa;
    sa.sa_family = AF_UNIX;
    strcpy(sa.sa_data, "socket.soc");
    if (sendto(sockfd, buf, sizeof(buf), 0, &sa, sizeof(sa)) == -1)
    {
        perror("sendto()\n");
        close(sockfd);
        return 1;
    }
    close(sockfd);
    return 0;
}