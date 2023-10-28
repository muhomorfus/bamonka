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
    socklen_t len = sizeof(sa);

    char sock_name[20];
    sprintf(sock_name, "%d.soc", getpid());

    struct sockaddr clsa;
    clsa.sa_family = AF_UNIX;
    strcpy(clsa.sa_data, sock_name);
    if (bind(sockfd, &clsa, sizeof(clsa)) == -1) 
    {
        perror("bind()\n");
        return 1;
    }

    if (sendto(sockfd, buf, sizeof(buf), 0, &sa, len) == -1)
    {
        perror("sendto()\n");
        unlink(sock_name);
        close(sockfd);
        return 1;
    }
    if (recvfrom(sockfd, buf, sizeof(buf), 0, &sa, &len) == -1) 
    {
        perror("recvfrom()\n");
        unlink(sock_name);
        close(sockfd);
        return 1;
    }
    printf("received: %s\n", buf);
    unlink(sock_name);
    close(sockfd);
    return 0;
}