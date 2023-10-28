#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUF_SIZE 50
#define CLIENT_NUM 5

int sockfd;

void signal_handler(int signal)
{
    close(sockfd);
    printf("exiting...\n");
    exit(0);
}

int main(int argc, char **argv) 
{
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("signal()\n");
        return 1;
    }
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        perror("socket()");
        return 1;
    }
    struct sockaddr sa;
    sa.sa_family = AF_UNIX;
    strcpy(sa.sa_data, "socket.soc");
    if (bind(sockfd, &sa, sizeof(sa)) == -1) 
    {
        perror("bind()");
        return 1;
    }
    listen(sockfd, 1);
    char buf[BUF_SIZE];
    while (1)
    {
        int newsockfd = accept(sockfd, NULL, NULL);
        if (newsockfd == -1)
        {
            perror("accept()\n");
            return 1;
        }

        if (read(newsockfd, buf, sizeof(buf)) == 0)
        {
            printf("client disconnected\n");
            continue;
        }
        printf("received: %s\n", buf);
        strcat(buf, "aaaa");
        write(newsockfd, buf, sizeof(buf));
        printf("sent: %s\n", buf);
    }

    return 0;
}