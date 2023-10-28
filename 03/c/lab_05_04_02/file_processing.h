#ifndef FILE_PROCESSING_H
#define FILE_PROCESSING_H

#include <stdio.h>

#include "return_code.h"
#include "product.h"

return_code f_insert_product(FILE *f, product_t product);
return_code insert_product(char *name);

return_code f_copy_sort_products(FILE *f_src, FILE *f_dst);
return_code copy_sort_products(char *name_src, char *name_dst);

return_code f_print_products_with_suffix(FILE *f, char *suffix);
return_code print_products_with_suffix(char *name, char *suffix);

#endif //FILE_PROCESSING_H
