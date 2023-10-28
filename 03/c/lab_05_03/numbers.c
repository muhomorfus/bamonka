#include <stdio.h>
#include <stdlib.h>

#include "numbers.h"
#include "return_code.h"

return_code count_numbers(FILE *f, size_t *n)
{
    if (fseek(f, 0L, SEEK_END) != 0)
        return exit_fseek_error;

    long int offset = ftell(f);
    if (offset == -1L)
        return exit_ftell_error;

    if (offset == 0)
        return exit_empty_file;

    if (offset % sizeof(int) != 0)
        return exit_invalid_file;

    *n = offset / sizeof(int);

    return exit_success;
}

return_code get_number_by_pos(FILE *f, size_t pos, int *number)
{
    return_code code;
    size_t n;
    code = count_numbers(f, &n);
    if (code != exit_success)
        return code;

    if (pos > n)
        return exit_file_overflow;

    if (fseek(f, pos * sizeof(int), SEEK_SET) != 0)
        return exit_fseek_error;

    if (fread(number, sizeof(int), 1, f) != 1)
        return exit_fread_error;

    return exit_success;
}

return_code put_number_by_pos(FILE *f, size_t pos, int number)
{
    return_code code;
    size_t n;
    code = count_numbers(f, &n);
    if (code != exit_success)
        return code;

    if (pos + 1 > n)
        return exit_file_overflow;

    if (fseek(f, pos * sizeof(int), SEEK_SET) != 0)
        return exit_fseek_error;

    if (fwrite(&number, sizeof(int), 1, f) != 1)
        return exit_fwrite_error;

    return exit_success;
}
