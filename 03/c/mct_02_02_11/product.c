#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "product.h"
#include "return_codes.h"

return_code count_products(FILE *f, size_t *n)
{
    long int start_offset = ftell(f);
    if (start_offset == -1L)
        return exit_tell_error;

    if (fseek(f, 0L, SEEK_END))
        return exit_seek_error;

    long int offset = ftell(f);
    if (offset == -1L)
        return exit_tell_error;

    if (offset % sizeof(product_t) != 0)
        return exit_file_error;

    *n = (size_t) offset / sizeof(product_t);

    if (fseek(f, start_offset, SEEK_SET))
        return exit_seek_error;

    return exit_success;
}

return_code get_product_by_pos(FILE *f, size_t pos, product_t *product)
{
    long int start_offset = ftell(f);
    if (start_offset == -1L)
        return exit_tell_error;

    size_t n;
    return_code code = count_products(f, &n);
    if (code != exit_success)
        return code;

    if (pos > n + 1)
        return exit_file_overflow;

    if (fseek(f, pos * sizeof(product_t), SEEK_SET))
        return exit_seek_error;

    if (fread(product, sizeof(product_t), 1, f) != 1)
        return exit_read_error;

    if (fseek(f, start_offset, SEEK_SET))
        return exit_seek_error;

    return exit_success;
}

return_code put_product_by_pos(FILE *f, size_t pos, product_t product)
{
    long int start_offset = ftell(f);
    if (start_offset == -1L)
        return exit_tell_error;

    size_t n;
    return_code code = count_products(f, &n);
    if (code != exit_success)
        return code;

    if (pos > n)
        return exit_file_overflow;

    if (fseek(f, pos * sizeof(product_t), SEEK_SET))
        return exit_seek_error;

    if (fwrite(&product, sizeof(product_t), 1, f) != 1)
        return exit_read_error;

    if (fseek(f, start_offset, SEEK_SET))
        return exit_seek_error;

    return exit_success;
}

return_code average_weight(FILE *f, double *avg)
{
    *avg = 0;
    size_t n;
    return_code code = count_products(f, &n);
    if (code != exit_success)
        return code;

    if (n == 0)
        return exit_empty_file;

    product_t product;
    for (size_t i = 0; i < n; i++)
    {
        code = get_product_by_pos(f, i, &product);
        if (code != exit_success)
            return code;

        *avg += product.weight;
    }

    *avg /= n;
    return exit_success;
}

return_code delete_lightest(FILE *f)
{
    double avg_weight;
    return_code code;
    code = average_weight(f, &avg_weight);
    if (code != exit_success)
        return code;

    size_t n;
    code = count_products(f, &n);
    if (code != exit_success)
        return code;

    size_t j = 0;
    product_t product;
    for (size_t i = 0; i < n; i++)
    {
        code = get_product_by_pos(f, i, &product);
        if (code != exit_success)
            return code;

        if (product.weight >= avg_weight)
        {
            code = put_product_by_pos(f, j++, product);
            if (code != exit_success)
                return code;
        }
    }

    if (j == n)
        return exit_success;

    product.weight = 0;
    strcpy(product.name, "0000");
    return put_product_by_pos(f, j, product);
}