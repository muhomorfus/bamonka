#ifndef MY_SNPRINTF_H
#define MY_SNPRINTF_H

#include <stdlib.h>
#include <stdarg.h>

int my_vsnprintf(char *restrict s, size_t n, const char *restrict format, va_list arg);
int my_snprintf(char *restrict s, size_t n, const char *restrict format, ...);

#endif //MY_SNPRINTF_H
