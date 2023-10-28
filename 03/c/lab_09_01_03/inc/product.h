#ifndef PRODUCT_H
#define PRODUCT_H

#include <stdbool.h>
#include <stddef.h>

#include "error.h"
#include "my_string.h"

#define PD_FILE_ERROR 2
#define PD_INPUT_ERROR 3
#define PD_ARGS_ERROR 4
#define PD_PRINT_ERROR 5
#define PD_INVALID_PRICE_ERROR 6
#define PD_INVALID_NAME_ERROR 7

typedef struct product_t
{
    my_string_t *name;
    int price;
} product_t;

product_t *pds_new(size_t len);
void pds_free(product_t *products, size_t len);

error_t pd_input(FILE *f, product_t *product);
error_t pd_print(FILE *f, product_t *product);

void pd_free_content(product_t *product);

#endif //PRODUCT_H
