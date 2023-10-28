#include <stdio.h>
#include <stdlib.h>

#include "short_string.h"
#include "error.h"
#include "product.h"

#ifndef ARGS_ERROR
#define ARGS_ERROR 10
#endif
#ifndef FILE_OPEN_ERROR
#define FILE_OPEN_ERROR 11
#endif
#ifndef NUMBER_ERROR
#define NUMBER_ERROR 12
#endif
#ifndef EMPTY_OUTPUT_ERROR
#define EMPTY_OUTPUT_ERROR 13
#endif

#define PRODUCTS_NUM 15

int main(int argc, char **argv)
{
    if (argc != 3)
        return ARGS_ERROR;

    FILE *f = fopen(argv[1], "r");
    if (!f)
        return FILE_OPEN_ERROR;

    double cost;
    error_t err;
    if (is_failure(err = to_double(argv[2], &cost)))
    {
        fclose(f);
        return err.code;
    }
    if (cost <= 0)
    {
        fclose(f);
        return ARGS_ERROR;
    }

    short_string_t buffer;
    if (is_failure(err = f_read_line(f, buffer)))
    {
        fclose(f);
        return err.code;
    }
    int n;
    if (is_failure(err = to_integer(buffer, &n)))
    {
        fclose(f);
        return err.code;
    }
    if (n <= 0)
    {
        fclose(f);
        return NUMBER_ERROR;
    }
    size_t len = (size_t) n;
    if (len > PRODUCTS_NUM)
    {
        fclose(f);
        return NUMBER_ERROR;
    }

    product_t products[PRODUCTS_NUM];

    for (size_t i = 0; i < len; i++)
        if (is_failure(err = f_input_product(f, &products[i])))
        {
            fclose(f);
            return err.code;
        }

    if (!feof(f))
    {
        fclose(f);
        return NUMBER_ERROR;
    }

    for (size_t i = 0; i < len; i++)
        if (products[i].price < cost)
        {
            print_product(products[i]);
            printf("\n");
        }

    fclose(f);
    return EXIT_SUCCESS;
}