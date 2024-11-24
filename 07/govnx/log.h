#ifndef LOG_H
#define LOG_H

typedef enum {
    LogFatal = 0,
    LogError = 1,
    LogWarn = 2,
    LogInfo = 3,
    LogDebug = 4,
} LogLevel;

void Log(LogLevel level, char* fmt, ...);

#endif //LOG_H
