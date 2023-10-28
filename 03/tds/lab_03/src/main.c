#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "usual_matrix.h"
#include "csc_matrix.h"
#include "ui.h"
#include "vector.h"
#include "error.h"

#define ERROR(err) printf("%d: %s (%s)\n", err.code, err.text, err.func);

int main(void)
{
    error_t err;
    err = main_scenario();
    if (is_failure(err))
        ERROR(err);

    return err.code;
}
