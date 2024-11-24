#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"

int currentLevel = LogWarn;

char *names[] = {
    [LogFatal] = "fatal",
    [LogError] = "error",
    [LogWarn] = "warn",
    [LogInfo] = "info",
    [LogDebug] = "debug",
};

void Log(LogLevel level, char* fmt, ...) {
    if (currentLevel < level) {
        return;
    }

    va_list arg;
    va_start(arg, fmt);

    char time_buffer[128];
    char log_str[1024];

    time_t rawTime;
    struct tm *local;

    time(&rawTime);
    local = localtime(&rawTime);
    strftime(time_buffer, 128, "%FT%H:%M:%SZ%z", local);

    sprintf(log_str, "{\"level\": \"%s\", \"time\": \"%s\", \"pid\": %d, \"message\": \"%s\"}\n", names[level], time_buffer, getpid(), fmt);
    vprintf(log_str, arg);
    va_end(arg);

    if (level == LogFatal) {
        exit(1);
    }
}

