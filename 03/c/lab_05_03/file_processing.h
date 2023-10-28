#ifndef FILE_PROCESSING_H
#define FILE_PROCESSING_H

#include <stdio.h>

#include "return_code.h"

return_code f_create_numbers_file(FILE *f, size_t n);
return_code create_numbers_file(char *filename, size_t number);

return_code f_print_numbers(FILE *f);
return_code print_numbers(char *filename);

return_code f_sort_numbers(FILE *f);
return_code sort_numbers(char *filename);

#endif //FILE_PROCESSING_H
