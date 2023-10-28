#include <stddef.h>

#include "error.h"

error_t new_error(const char *text, const int code)
{
    error_t err = { text, code };
    return err;
}

error_t new_default_failure(const char *text)
{
    return new_error(text, CODE_FAILURE_DEFAULT);
}

error_t new_success()
{
    return new_error(NULL, CODE_SUCCESS);
}

bool is_failure(error_t err)
{
    return err.code != CODE_SUCCESS;
}

