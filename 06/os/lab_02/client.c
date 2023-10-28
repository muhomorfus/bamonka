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

int sockfd;

void signal_handler(int signal)
{
    close(sockfd);
    printf("exiting...\n");
    exit(0);
}

int main(int argc, char **argv)
{
    if ((signal(SIGINT, signal_handler) == SIG_ERR)) {
        perror("signal()\n");
        return 1;
    }
    if (argc != 3)
    {
        perror("args not enough\n");
        return 1;
    }
    struct hostent* server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        perror("gethostbyname()\n");
        return 1;
    }
    short port = (short) atoi(argv[2]);
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr = *((struct in_addr*) server->h_addr_list[0]);
    sa.sin_port = htons(port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        perror("socket()");
        return 1;
    }
    if (connect(sockfd, (struct sockaddr*) &sa, sizeof(sa)) == -1)
    {
        perror("connect()");
        return 1;
    }
    char buf[BUF_SIZE];
    while (1) 
    {
        if (scanf("%s", buf) != 1)
        {
            perror("scanf()");
            return 1;
        }
        sprintf(buf, "%s %d", buf, getpid());
        write(sockfd, buf, sizeof(buf));
        read(sockfd, buf, sizeof(buf));
        printf("received: %s\n", buf);
    }
    return 0;
}