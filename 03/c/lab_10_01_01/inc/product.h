#ifndef PRODUCT_H
#define PRODUCT_H

#include <stdbool.h>
#include <stddef.h>

#include "my_string.h"
#include "node.h"

#define PD_FILE_ERROR 2
#define PD_INPUT_ERROR 3
#define PD_ARGS_ERROR 4
#define PD_PRINT_ERROR 5
#define PD_INVALID_PRICE_ERROR 6
#define PD_INVALID_NAME_ERROR 7
#define PD_ALLOC_ERROR 8

typedef struct product_t
{
    my_string_t *name;
    int price;
} product_t;

product_t *pds_new(size_t len);
void pds_free(product_t *products, size_t len);

product_t *pds_from_file(FILE *f, size_t *len);
node_t *pds_list_from_arr(product_t *products, size_t len);

int pd_input(FILE *f, product_t *product);
int pd_print(FILE *f, product_t *product);

void pd_free_content(product_t *product);

int pd_compare_by_name(const void *a, const void *b);
int pd_compare_by_name_then_price(const void *a, const void *b);
int pd_compare_by_price_then_name(const void *a, const void *b);

#endif //PRODUCT_H
