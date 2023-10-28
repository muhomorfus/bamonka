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
#include <poll.h>

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
    if (argc != 2)
    {
        perror("args not enough\n");
        return 1;
    }
    short port = (short) atoi(argv[1]);
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("signal()\n");
        return 1;
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        perror("socket()");
        return 1;
    }
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr*) &sa, sizeof(sa)) == -1) 
    {
        perror("bind()");
        return 1;
    }
    listen(sockfd, CLIENT_NUM);
    char buf[BUF_SIZE];
    int cur = 1;

    struct pollfd pfd[CLIENT_NUM + 1];
    pfd[0].fd = sockfd;
    pfd[0].events = POLLIN;

    while (1)
    {
        int rc = poll(pfd, cur, 10000);
        if (rc == -1)
        {
            perror("poll()\n");
            return 1;
        }
        if (rc != 0)
        {
            if (pfd[0].revents) 
            {
                pfd[0].revents = 0;
                pfd[cur].fd = accept(sockfd, NULL, NULL);
                if (pfd[cur].fd == -1)
                {
                    perror("accept()");
                    return 1;
                }
                pfd[cur].events = POLLIN;
                cur++; 
            } 
            else
            {
                for (int i = 1; i < cur; i++)
                {
                    if (pfd[i].revents) 
                    {
                        pfd[i].revents = 0;
                        if (read(pfd[i].fd, buf, sizeof(buf)) == 0)
                            pfd[i].fd = 0;
                        else
                        {
                            printf("received: %s\n", buf);
                            sprintf(buf, "%s %d", buf, getpid());
                            write(pfd[i].fd, buf, sizeof(buf));
                            printf("sent: %s\n", buf);
                        }
                    }
                }
            }
        }
    }

    return 0;
}