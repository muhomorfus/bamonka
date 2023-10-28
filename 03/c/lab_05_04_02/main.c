#include <stdio.h>
#include <string.h>

#include "file_processing.h"
#include "product.h"
#include "return_code.h"

int main(int argc, char **argv)
{
    if (argc < 2)
        return exit_not_enough_args;

    if (strcmp(argv[1], "sb") == 0)
    {
        if (argc != 4)
            return exit_not_enough_args;

        return copy_sort_products(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "fb") == 0)
    {
        if (argc != 4)
            return exit_not_enough_args;

        if (strlen(argv[3]) > PRODUCT_NAME_SIZE)
            return exit_invalid_args;

        return print_products_with_suffix(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "ab") == 0)
    {
        if (argc != 3)
            return exit_not_enough_args;

        return insert_product(argv[2]);
    }

    return exit_invalid_args;
}