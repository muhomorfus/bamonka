#ifndef SHORT_STRING_H
#define SHORT_STRING_H

#include <stdbool.h>

#include "error.h"

#define SHORT_STRING_SIZE 257

#ifndef FILE_ERROR
#define FILE_ERROR 2
#endif
#ifndef INPUT_ERROR
#define INPUT_ERROR 3
#endif
#ifndef ARGS_ERROR
#define ARGS_ERROR 4
#endif
#ifndef OVERFLOW_ERROR
#define OVERFLOW_ERROR 5
#endif
#ifndef CONVERSION_ERROR
#define CONVERSION_ERROR 6
#endif

typedef char short_string_t[SHORT_STRING_SIZE];

error_t f_read_line(FILE *f, short_string_t str);

bool is_double(char *str);
error_t to_double(char *str, double *res);

bool is_integer(char *str);
error_t to_integer(char *str, int *res);

bool is_empty(char *str);

#endif //SHORT_STRING_H
