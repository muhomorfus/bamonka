#include <stdio.h>
#include <stdlib.h>

#include "my_string.h"
#include "error.h"
#include "product.h"

#define ARGS_ERROR 10
#define FILE_OPEN_ERROR 11
#define NUMBER_ERROR 12
#define EMPTY_OUTPUT_ERROR 13
#define ALLOC_ERROR 14

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

    my_string_t *buffer = ms_new();
    if (is_failure(err = ms_read_line(f, buffer)))
    {
        fclose(f);
        ms_free(buffer);
        return err.code;
    }
    int n;
    if (is_failure(err = to_integer(buffer->ptr, &n)))
    {
        fclose(f);
        ms_free(buffer);
        return err.code;
    }
    ms_free(buffer);

    if (n <= 0)
    {
        fclose(f);
        return NUMBER_ERROR;
    }
    size_t len = (size_t) n;

    product_t *products = pds_new(len);
    if (!products)
    {
        fclose(f);
        return ALLOC_ERROR;
    }

    for (size_t i = 0; i < len; i++)
        if (is_failure(err = pd_input(f, &products[i])))
        {
            fclose(f);
            pds_free(products, len);
            return err.code;
        }

    if (!feof(f))
    {
        fclose(f);
        pds_free(products, len);
        return NUMBER_ERROR;
    }

    for (size_t i = 0; i < len; i++)
        if (products[i].price < cost)
        {
            pd_print(stdout, &products[i]);
            printf("\n");
        }

    pds_free(products, len);
    fclose(f);
    return EXIT_SUCCESS;
}