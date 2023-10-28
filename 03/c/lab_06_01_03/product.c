#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "product.h"
#include "error.h"
#include "short_string.h"

error_t f_input_product(FILE *f, product_t *product)
{
    if (!f)
        return new_error("file is null", INPUT_ERROR);
    if (!product)
        return new_error("pointer to product is null", ARGS_ERROR);

    error_t err;
    short_string_t buffer;
    if (is_failure(err = f_read_line(f, buffer)))
        return err;
    if (strlen(buffer) >= PRODUCT_NAME_SIZE)
        return new_error("product name is too long", INVALID_NAME_ERROR);
    strcpy(product->name, buffer);

    if (is_failure(err = f_read_line(f, buffer)))
        return err;

    if (is_failure(err = to_integer(buffer, &product->price)))
        return err;

    if (product->price <= 0)
        return new_error("invalid price", INVALID_PRICE_ERROR);

    return new_success();
}

error_t f_print_product(FILE *f, product_t product)
{
    if (!f)
        return new_error("file is null", INPUT_ERROR);

    if (printf("%s\n", product.name) < 0)
        return new_error("product name print error", PRINT_ERROR);
    if (printf("%d", product.price) < 0)
        return new_error("product price print error", PRINT_ERROR);

    return new_success();
}

error_t print_product(product_t product)
{
    return f_print_product(stdout, product);
}