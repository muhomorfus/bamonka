#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine.h"
#include "error.h"

#define ERROR(err) printf("%d: %s (%s)\n", err.code, err.text, err.func);

int main(void)
{
    error_t err;
    err = eng_work();
    if (is_failure(err))
        ERROR(err);

    return err.code;
}
