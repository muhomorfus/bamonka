#ifndef NET_H
#define NET_H

typedef enum {
    MethodGet = 0,
    MethodHead = 1,
} Method;

typedef struct {
    Method Method;
    char URI[1024];
    char Version[16];
    int ResponseWriter;
} Request;

int ParseRequest(char *raw, Request *req);
int Handle(Request *req);

#endif //NET_H
