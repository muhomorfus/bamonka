#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "file_processing.h"
#include "return_code.h"
#include "numbers.h"

return_code f_create_numbers_file(FILE *f, size_t n)
{
    if (fseek(f, 0L, SEEK_SET) != 0)
        return exit_fseek_error;

    int number;
    for (size_t i = 0; i < n; i++)
    {
        number = rand();
        if (fwrite(&number, sizeof(int), 1, f) != 1)
            return exit_fwrite_error;
    }


    return exit_success;
}

return_code create_numbers_file(char *filename, size_t number)
{
    FILE *f = fopen(filename, "wb");
    if (f == NULL)
        return exit_fopen_error;

    return_code code = f_create_numbers_file(f, number);
    if (code != exit_success)
        return code;

    fclose(f);
    return exit_success;
}

return_code f_print_numbers(FILE *f)
{
    return_code code;

    size_t n;
    code = count_numbers(f, &n);
    if (code != exit_success)
        return code;

    int number;
    for (size_t i = 0; i < n; i++)
    {
        code = get_number_by_pos(f, i, &number);
        if (code != exit_success)
            return code;

        printf("%d\n", number);
    }

    return exit_success;
}

return_code print_numbers(char *filename)
{
    FILE *f = fopen(filename, "rb+");
    if (f == NULL)
        return exit_fopen_error;

    return_code code = f_print_numbers(f);
    if (code != exit_success)
        return code;

    fclose(f);
    return exit_success;
}

return_code f_sort_numbers(FILE *f)
{
    size_t n;
    return_code code;
    code = count_numbers(f, &n);
    if (code != exit_success)
        return code;

    int a, b;
    for (size_t i = 0; i < n - 1; i++)
        for (size_t j = 0; j < n - i - 1; j++)
        {
            code = get_number_by_pos(f, j, &a);
            if (code != exit_success)
                return code;

            code = get_number_by_pos(f, j + 1, &b);
            if (code != exit_success)
                return code;

            if (a > b)
            {
                code = put_number_by_pos(f, j, b);
                if (code != exit_success)
                    return code;

                code = put_number_by_pos(f, j + 1, a);
                if (code != exit_success)
                    return code;
            }
        }

    return exit_success;
}

return_code sort_numbers(char *filename)
{
    FILE *f = fopen(filename, "rb+");
    if (f == NULL)
        return exit_fopen_error;

    return_code code = f_sort_numbers(f);
    if (code != exit_success)
        return code;

    fclose(f);
    return exit_success;
}
