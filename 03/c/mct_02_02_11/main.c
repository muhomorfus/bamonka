#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "return_codes.h"
#include "product.h"

int main(int argc, char **argv)
{
    if (argc < 2)
        return exit_invalid_args;

    FILE *f = fopen(argv[1], "rb+");
    if (f == NULL)
        return exit_file_open_error;

    return_code code = delete_lightest(f);

    fclose(f);
    return code;
}
