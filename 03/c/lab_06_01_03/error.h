#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

#define CODE_SUCCESS 0
#define CODE_FAILURE_DEFAULT 1

typedef struct 
{
    const char *text;
    int code;
} error_t;

error_t new_error(const char *text, int code);
error_t new_default_failure(const char *text);
error_t new_success();

bool is_failure(error_t err);

#endif
