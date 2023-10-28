#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>

#define BUF_SIZE 50

int main(int argc, char **argv)
{
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        perror("socket()");
        return 1;
    }
    struct sockaddr sa;
    sa.sa_family = AF_UNIX;
    strcpy(sa.sa_data, "socket.soc");
    if (connect(sockfd, (struct sockaddr*) &sa, sizeof(sa)) == -1)
    {
        perror("connect()");
        return 1;
    }
    char buf[BUF_SIZE];
    sprintf(buf, "%d", getpid());

    write(sockfd, buf, sizeof(buf));
    read(sockfd, buf, sizeof(buf));
    printf("received: %s\n", buf);
    return 0;
}