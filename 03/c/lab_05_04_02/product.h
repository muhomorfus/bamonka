#ifndef PRODUCT_H
#define PRODUCT_H

#include <inttypes.h>
#include <stdio.h>

#include "return_code.h"

#define PRODUCT_NAME_SIZE 31
#define PRODUCT_MANUFACTURER_SIZE 16

typedef struct
{
    char name[PRODUCT_NAME_SIZE];
    char manufacturer[PRODUCT_MANUFACTURER_SIZE];
    uint32_t price;
    uint32_t amount;
} product_t;

return_code read_product(product_t *product);
void print_product(product_t product);
return_code count_products(FILE *f, size_t *number);
return_code get_product_by_pos(FILE *f, size_t pos, product_t *product);
return_code put_product_by_pos(FILE *f, size_t pos, product_t product);
return_code insert_product_to_pos(FILE *f, size_t pos, product_t product);

#endif //PRODUCT_H
