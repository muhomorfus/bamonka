#include <stddef.h>

#include "error.h"

error_t new_error(const char *text, const int code, const char *func)
{
    error_t err = { text, code, func };
    return err;
}

error_t new_success(const char *func)
{
    return new_error(NULL, CODE_SUCCESS, func);
}

bool is_failure(error_t err)
{
    return err.code != CODE_SUCCESS;
}

