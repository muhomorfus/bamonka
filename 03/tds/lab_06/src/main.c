#include <stdio.h>

#include "engine.h"
#include "error.h"

#define ERROR(err) printf("Ошибка %d: %s.\n", err.code, err.text);

int main(void)
{
    error_t err;
    err = eng_work();
    if (is_failure(err))
        ERROR(err);

    return err.code;
}
