#ifndef SHORT_STRING_H
#define SHORT_STRING_H

#include <stdbool.h>
#include <stdio.h>

#define MS_FILE_ERROR 2
#define MS_INPUT_ERROR 3
#define MS_ARGS_ERROR 4
#define MS_OVERFLOW_ERROR 5
#define MS_CONVERSION_ERROR 6
#define MS_ALLOCATION_ERROR 7

typedef struct my_string_t
{
    char *ptr;
    size_t allocated;
} my_string_t;

my_string_t *ms_new();
void ms_free(my_string_t *str);

int ms_compact(my_string_t *str);

int ms_read_line(FILE *f, my_string_t *str);

int to_double(char *str, double *res);
int to_integer(char *str, int *res);

#endif //SHORT_STRING_H
