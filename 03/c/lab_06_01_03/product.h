#ifndef PRODUCT_H
#define PRODUCT_H

#include <stdbool.h>
#include <stddef.h>

#include "error.h"

#define PRODUCT_NAME_SIZE 26

#ifndef FILE_ERROR
#define FILE_ERROR 2
#endif
#ifndef INPUT_ERROR
#define INPUT_ERROR 3
#endif
#ifndef ARGS_ERROR
#define ARGS_ERROR 4
#endif
#ifndef PRINT_ERROR
#define PRINT_ERROR 5
#endif
#ifndef INVALID_PRICE_ERROR
#define INVALID_PRICE_ERROR 6
#endif
#ifndef INVALID_NAME_ERROR
#define INVALID_NAME_ERROR 7
#endif

typedef struct
{
    char name[PRODUCT_NAME_SIZE];
    int price;
} product_t;

error_t f_input_product(FILE *f, product_t *product);
error_t f_print_product(FILE *f, product_t product);

error_t print_product(product_t product);

#endif //PRODUCT_H
