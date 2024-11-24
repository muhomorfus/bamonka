#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <sys/epoll.h>

#include "net.h"
#include "log.h"

#define poolSize 16
#define maxEvents 8

int socketFD;

int running = 1;

void exitHandler(int signal) {
    Log(LogWarn, "triggered exiting");
    running = 0;
}

void acceptNewConnection(int epollFD) {
    int newSocketFD = accept(socketFD, NULL, NULL);
    if (newSocketFD < 0) {
        Log(LogError, "accept failed");
        return;
    }

    struct epoll_event e;
    e.events = EPOLLIN | EPOLLET;
    e.data.fd = newSocketFD;
    if (epoll_ctl(epollFD, EPOLL_CTL_ADD, newSocketFD, &e) < 0) {
        Log(LogFatal, "cant add socket");
    }

    Log(LogDebug, "new connection accepted");
}

void processSocketData(int fd) {
    char buf[4096] = {0};

    ssize_t n = read(fd, buf, sizeof(buf));
    if (n < 0) {
        Log(LogError, "cant read");
        return;
    }

    Request req;
    req.ResponseWriter = fd;
    int rc = ParseRequest(buf, &req);
    if (rc != 0) {
        Log(LogError, "cant parse request");
        return;
    }

    rc = Handle(&req);
    if (rc != 0) {
        Log(LogError, "cant handle request");
        return;
    }
}

void process() {
    int epollFD = epoll_create1(0);
    if (epollFD < 0) {
        Log(LogFatal, "cant create epoll");
    }

    struct epoll_event e;
    e.events = EPOLLIN;
    e.data.fd = socketFD;
    if (epoll_ctl(epollFD, EPOLL_CTL_ADD, socketFD, &e) < 0) {
        Log(LogFatal, "cant add main socket to epoll");
    }

    Log(LogDebug, "starting processing connections");

    struct epoll_event events[maxEvents];

    while (running) {
        int n = epoll_wait(epollFD, events, maxEvents, -1);
        if (n < 0) {
            if (!running) {
               return;
            }
            Log(LogFatal, "epoll failed");
        }

        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == socketFD) {
                acceptNewConnection(epollFD);
                continue;
            }

            processSocketData(events[i].data.fd);
        }
    }
}

int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IOLBF, 1024);

    if (signal(SIGINT, exitHandler) == SIG_ERR) {
        Log(LogFatal, "cant create signal handler for sigint");
    }
    if (signal(SIGTERM, exitHandler) == SIG_ERR) {
        Log(LogFatal, "cant create signal handler for sigterm");
    }

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
        Log(LogFatal, "cant create socket");
    }

    fcntl(socketFD, F_SETFL, O_NONBLOCK);

    short port = 80;
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(port);
    if (bind(socketFD, (struct sockaddr *) &sa, sizeof(sa)) == -1) {
        Log(LogFatal, "cant bind socket");
    }

    listen(socketFD, 65536);

    int rc;

    for (int i = 0; i < poolSize; i++) {
        rc = fork();
        if (rc < 0) {
            Log(LogFatal, "cant fork");
        }
        if (rc == 0) {
            process();
            return 0;
        }
    }

    Log(LogInfo, "forks are started");
    process();

    close(socketFD);
    return 0;
}