#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#include "product.h"
#include "return_code.h"
#include "string_functions.h"

return_code read_product(product_t *product)
{
    return_code code;

    code = read_line(product->name, PRODUCT_NAME_SIZE);
    if (code != exit_success)
        return code;

    code = read_line(product->manufacturer, PRODUCT_MANUFACTURER_SIZE);
    if (code != exit_success)
        return code;

    if (scanf("%"SCNu32, &(product->price)) != 1)
        return exit_invalid_input;

    if (scanf("%"SCNu32, &(product->amount)) != 1)
        return exit_invalid_input;

    return exit_success;
}

void print_product(product_t product)
{
    printf("%s\n", product.name);
    printf("%s\n", product.manufacturer);
    printf("%"PRIu32"\n", product.price);
    printf("%"PRIu32"\n", product.amount);
}

return_code count_products(FILE *f, size_t *number)
{
    if (fseek(f, 0L, SEEK_END) != 0)
        return exit_fseek_error;

    long int end_offset = ftell(f);
    if (end_offset == -1L)
        return exit_ftell_error;

    if (end_offset % sizeof(product_t) != 0)
        return exit_invalid_file;

    *number = end_offset / sizeof(product_t);

    if (end_offset == 0)
        return exit_empty_file;

    return exit_success;
}

return_code get_product_by_pos(FILE *f, size_t pos, product_t *product)
{
    return_code code;

    size_t num = 0;
    code = count_products(f, &num);
    if (code != exit_success)
        return code;

    if (pos >= num)
        return exit_file_overflow;

    if (fseek(f, pos * sizeof(product_t), SEEK_SET) != 0)
        return exit_fseek_error;

    if (fread(product, sizeof(product_t), 1, f) != 1)
        return exit_fread_error;

    return exit_success;
}

return_code put_product_by_pos(FILE *f, size_t pos, product_t product)
{
    return_code code;

    size_t num = 0;
    code = count_products(f, &num);
    if (code != exit_success && code != exit_empty_file)
        return code;

    if (pos > num)
        return exit_file_overflow;

    if (fseek(f, pos * sizeof(product_t), SEEK_SET) != 0)
        return exit_fseek_error;

    if (fwrite(&product, sizeof(product_t), 1, f) != 1)
        return exit_fwrite_error;

    return exit_success;
}

return_code insert_product_to_pos(FILE *f, size_t pos, product_t product)
{
    return_code code;

    size_t num = 0;
    code = count_products(f, &num);
    if (code != exit_success && code != exit_empty_file)
        return code;

    for (size_t i = num; i > pos; i--)
    {
        product_t p;
        code = get_product_by_pos(f, i - 1, &p);
        if (code != exit_success)
            return code;

        code = put_product_by_pos(f, i, p);
        if (code != exit_success)
            return code;
    }

    return put_product_by_pos(f, pos, product);
}

