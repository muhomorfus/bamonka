#ifndef PRODUCT_H
#define PRODUCT_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "return_codes.h"

#define PRODUCT_NAME_LEN 126

#pragma pack(push, 1)
typedef struct
{
    char name[PRODUCT_NAME_LEN];
    uint32_t weight;
} product_t;
#pragma pack(pop)

return_code count_products(FILE *f, size_t *n);
return_code get_product_by_pos(FILE *f, size_t pos, product_t *product);
return_code put_product_by_pos(FILE *f, size_t pos, product_t product);
return_code average_weight(FILE *f, double *avg);
return_code delete_lightest(FILE *f);

#endif //PRODUCT_H
