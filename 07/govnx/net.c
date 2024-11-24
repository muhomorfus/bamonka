#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "net.h"
#include "log.h"

char *methodsMap[] = {
    [MethodGet] = "GET",
    [MethodHead] = "HEAD",
};

typedef enum {
    ResponseCodeOk = 0,
    ResponseCodeForbidden = 1,
    ResponseCodeNotFound = 2,
    ResponseCodeMethodNotAllowed = 3,
    ResponseCodeBadRequest = 4,
    ResponseCodeInternalServerError = 5,
} ResponseCode;

char *commentsMap[] = {
    [ResponseCodeOk] = "200 OK",
    [ResponseCodeForbidden] = "403 Forbidden",
    [ResponseCodeNotFound] = "404 Not Found",
    [ResponseCodeMethodNotAllowed] = "405 Method Not Allowed",
    [ResponseCodeBadRequest] = "400 Bad Request",
    [ResponseCodeInternalServerError] = "500 Internal Server Error",
};

int writeStatusLine(Request *req, ResponseCode code) {
    char buf[64] = { 0 };
    if (sprintf(buf, "%s %s\n", req->Version, commentsMap[code]) < 0) {
        Log(LogFatal, "unknown error with sprintf");
        return -1;
    }

    if (write(req->ResponseWriter, buf, strlen(buf)) == -1) {
        Log(LogError, "cant write to socket");
        return -1;
    }

    return 0;
}

int writeHeaders(Request *req, char *contentType, off_t size) {
    char contentLengthHeader[128] = {0 };
    sprintf(contentLengthHeader, "Content-Length: %ld\n", size);
    if (write(req->ResponseWriter, contentLengthHeader, strlen(contentLengthHeader)) == -1) {
        Log(LogError, "cant write to socket");
        return -1;
    }

    if (contentType != NULL) {
        char contentTypeHeader[128] = {0 };
        sprintf(contentTypeHeader, "Content-Type: %s\n", contentType);
        if (write(req->ResponseWriter, contentTypeHeader, strlen(contentTypeHeader)) == -1) {
            Log(LogError, "cant write to socket");
            return -1;
        }
    }

    if (write(req->ResponseWriter, "\n", 1) == -1) {
        Log(LogError, "cant write to socket");
        return -1;
    }

    return 0;
}

int writeError(Request *req, ResponseCode code) {
    int rc;
    rc = writeStatusLine(req, code);
    if (rc != 0) {
        Log(LogError, "cant write status");
        return rc;
    }

    char message[128];
    sprintf(message, "{\"error\": true, \"status\": \"%s\"}", commentsMap[code]);

    if (writeHeaders(req, "application/json", strlen(message)) < 0) {
        Log(LogError, "cant write headers");
        return -1;
    }

    if (write(req->ResponseWriter, message, strlen(message)) == -1) {
        Log(LogError, "cant write to socket");
        return -1;
    }

    return 0;
}

int ParseRequest(char *raw, Request *req) {
    char method[16] = {0 };

    if (sscanf(raw, "%s %s %s\n", method, req->URI, req->Version) != 3) {
        Log(LogError, "invalid request");
        return -1;
    }

    if (strstr(req->Version, "HTTP/1.") == NULL) {
        Log(LogError, "invalid http version: %s", req->Version);
        return -1;
    }

    int len = sizeof(methodsMap) / sizeof(char*);

    for (int i = 0; i < len; i++) {
        if (strcmp(method, methodsMap[i]) == 0) {
            req->Method = i;
            return 0;
        }
    }

    Log(LogWarn, "unsupported method: %s", req->Method);
    writeError(req, ResponseCodeMethodNotAllowed);
    return -1;
}

int copy(int dst, int src) {
    ssize_t n = 1;
    while (n > 0) {
        char buf[4096] = { 0 };

        n = read(src, buf, 4096);


        if (write(dst, buf, n) < 0) {
            Log(LogError, "cant write to dst");
            return -1;
        }
    }

    if (n < 0) {
        Log(LogError, "cant read from src");
        return -1;
    }

    return 0;
}

typedef struct {
    char *extension;
    char *contentType;
} extensionToContentTypeMapping;

extensionToContentTypeMapping extensionToContentType[] = {
    {
        .extension = ".html",
        .contentType = "text/html",
    },
    {
        .extension = ".css",
        .contentType = "text/css",
    },
    {
        .extension = ".js",
        .contentType = "text/javascript",
    },
    {
        .extension = ".png",
        .contentType = "image/png",
    },
    {
        .extension = ".jpg",
        .contentType = "image/jpeg",
    },
    {
        .extension = ".jpeg",
        .contentType = "image/jpeg",
    },
    {
        .extension = ".swf",
        .contentType = "application/vnd.adobe.flash-movie",
    },
    {
        .extension = ".gif",
        .contentType = "image/gif",
    },
};

char *detectContentType(char *filename) {
    int len = sizeof(extensionToContentType) / sizeof(extensionToContentTypeMapping);

    for (int i = 0; i < len; i++) {
        if (strcmp(filename + strlen(filename) - strlen(extensionToContentType[i].extension), extensionToContentType[i].extension) == 0) {
            return extensionToContentType[i].contentType;
        }
    }

    return NULL;
}

int handleStatic(Request *req, char *filename) {
    if (strstr(filename, "../") != NULL || filename[0] == '/') {
        writeError(req, ResponseCodeForbidden);
        Log(LogWarn, "forbidden path");
        return -1;
    }

    if (access(filename, F_OK) != 0) {
        writeError(req, ResponseCodeNotFound);
        Log(LogWarn, "file %s not found", filename);
        return -1;
    }

    struct stat info;
    if (stat(filename, &info) < 0) {
        writeError(req, ResponseCodeInternalServerError);
        Log(LogError, "unknown error with fs");
        return -1;
    }

    if (!S_ISREG(info.st_mode)) {
        writeError(req, ResponseCodeBadRequest);
        Log(LogWarn, "file %s is not supported", filename);
        return -1;
    }

    writeStatusLine(req, ResponseCodeOk);

    int rc = writeHeaders(req, detectContentType(filename), info.st_size);
    if (rc != 0) {
        return rc;
    }

    if (req->Method == MethodHead) {
        return 0;
    }

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        writeError(req, ResponseCodeInternalServerError);
        Log(LogError, "unknown error with fs");
        return -1;
    }

    rc = copy(req->ResponseWriter, fd);
    if (rc != 0) {
        close(fd);
        return rc;
    }

    close(fd);

    return 0;
}

int Handle(Request *req) {
    Log(LogDebug, "%s %s", methodsMap[req->Method], req->URI);

    if (strcmp(req->URI, "/") == 0) {
        return handleStatic(req, "index.html");
    }

    char *filename = &(req->URI[1]);

    return handleStatic(req, filename);
}

