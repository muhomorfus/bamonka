#include <stdio.h>
#include <stdbool.h>

#include "file_processing.h"
#include "product.h"
#include "return_code.h"
#include "string_functions.h"

return_code f_insert_product(FILE *f, product_t product)
{
    return_code code;
    size_t num = 0;

    code = count_products(f, &num);
    if (code != exit_success && code != exit_empty_file)
        return code;

    bool less = true;
    size_t i = 0;
    for (; i < num && less; i++)
    {
        product_t p;
        code = get_product_by_pos(f, i, &p);
        if (code != exit_success)
            return code;

        if (product.price > p.price)
            less = false;
        else if (product.price == p.price)
            if (product.amount > p.amount)
                less = false;
    }

    if (i > 0 && !less)
        i -= 1;

    return insert_product_to_pos(f, i, product);
}

return_code insert_product(char *name)
{
    FILE *f = fopen(name, "rb+");
    if (f == NULL)
        return exit_fopen_error;

    product_t product = { 0 };
    return_code code;
    code = read_product(&product);
    if (code != exit_success)
    {
        fclose(f);
        return code;
    }

    code = f_insert_product(f, product);
    fclose(f);
    return code;
}

return_code f_copy_sort_products(FILE *f_src, FILE *f_dst)
{
    size_t num_src = 0;
    return_code code;
    code = count_products(f_src, &num_src);
    if (code != exit_success)
        return code;

    size_t num_dst = 0;
    code = count_products(f_dst, &num_dst);
    if (code != exit_empty_file)
        return exit_dst_not_empty;

    for (size_t i = 0; i < num_src; i++)
    {
        product_t product;
        code = get_product_by_pos(f_src, i, &product);
        if (code != exit_success)
            return code;

        code = f_insert_product(f_dst, product);
        if (code != exit_success)
            return code;
    }

    return exit_success;
}

return_code copy_sort_products(char *name_src, char *name_dst)
{
    FILE *f_src = fopen(name_src, "rb");
    if (f_src == NULL)
        return exit_fopen_error;

    FILE *f_dst = fopen(name_dst, "wb+");
    if (f_dst == NULL)
    {
        fclose(f_src);
        return exit_fopen_error;
    }

    return_code code = f_copy_sort_products(f_src, f_dst);
    fclose(f_src);
    fclose(f_dst);

    return code;
}

return_code f_print_products_with_suffix(FILE *f, char *suffix)
{
    return_code code;
    size_t num = 0;
    code = count_products(f, &num);
    if (code != exit_success)
        return code;

    int counter = 0;
    for (size_t i = 0; i < num; i++)
    {
        product_t product;
        code = get_product_by_pos(f, i, &product);
        if (code != exit_success)
            return code;

        if (has_suffix(product.name, suffix))
        {
            print_product(product);
            counter++;
        }
    }

    if (counter == 0)
        return exit_no_solution;

    return exit_success;
}

return_code print_products_with_suffix(char *name, char *suffix)
{
    FILE *f = fopen(name, "rb");
    if (f == NULL)
        return exit_fopen_error;

    return_code code;
    code = f_print_products_with_suffix(f, suffix);
    fclose(f);
    return code;
}
