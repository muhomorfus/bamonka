#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "product.h"
#include "error.h"
#include "my_string.h"

error_t pd_input(FILE *f, product_t *product)
{
    if (!f)
        return NEW_ERROR("file is null", PD_INPUT_ERROR);
    if (!product)
        return NEW_ERROR("pointer to product is null", PD_ARGS_ERROR);

    error_t err;
    product->name = ms_new();
    if (is_failure(err = ms_read_line(f, product->name)))
        return err;

    my_string_t *buffer = ms_new();
    if (is_failure(err = ms_read_line(f, buffer)))
    {
        ms_free(buffer);
        return err;
    }

    if (is_failure(err = to_integer(buffer->ptr, &product->price)))
    {
        ms_free(buffer);
        return err;
    }

    ms_free(buffer);
    if (product->price <= 0)
        return NEW_ERROR("invalid price", PD_INVALID_PRICE_ERROR);

    return NEW_SUCCESS();
}

error_t pd_print(FILE *f, product_t *product)
{
    if (!f)
        return NEW_ERROR("file is null", PD_INPUT_ERROR);
    if (!product)
        return NEW_ERROR("pointer to product is null", PD_ARGS_ERROR);

    if (printf("%s\n", product->name->ptr) < 0)
        return NEW_ERROR("product name print error", PD_PRINT_ERROR);
    if (printf("%d", product->price) < 0)
        return NEW_ERROR("product price print error", PD_PRINT_ERROR);

    return NEW_SUCCESS();
}

void pd_free_content(product_t *product)
{
    if (product)
    {
        ms_free(product->name);
        product->name = NULL;
    }
}

void pds_free(product_t *products, size_t len)
{
    for (size_t i = 0; i < len; i++)
        pd_free_content(&products[i]);
    free(products);
}

product_t *pds_new(size_t len)
{
    return calloc(len, sizeof(product_t));
}