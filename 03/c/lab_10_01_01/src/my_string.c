#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "my_string.h"

my_string_t *ms_new()
{
    my_string_t *str = calloc(1, sizeof(my_string_t));
    return str;
}

void ms_free(my_string_t *str)
{
    if (str)
    {
        free(str->ptr);
        str->allocated = 0;
        free(str);
    }
}

int ms_compact(my_string_t *str)
{
    if (!str)
        return MS_ARGS_ERROR;
    if (!str->ptr)
        return EXIT_SUCCESS;

    size_t new_size = strlen(str->ptr) + 1;
    char *res = realloc(str->ptr, new_size * sizeof(char));
    if (!res)
        return MS_ALLOCATION_ERROR;

    str->ptr = res;
    str->allocated = new_size;
    return EXIT_SUCCESS;
}

bool is_empty(char *str)
{
    if (!str)
        return true;

    for (size_t i = 0; str[i]; i++)
        if (!isspace(str[i]))
            return false;
    return true;
}

int increase_size(my_string_t *str)
{
    if (!str)
        return MS_ARGS_ERROR;

    size_t new_size = str->allocated * 2 + 2;
    char *res = realloc(str->ptr, new_size * sizeof(char));
    if (!res)
        return MS_ALLOCATION_ERROR;
    str->ptr = res;
    str->allocated = new_size;

    return EXIT_SUCCESS;
}

int append_symbol(my_string_t *str, char c)
{
    if (!str)
        return MS_ARGS_ERROR;

    size_t len = 0;
    if (str->ptr)
        len = strlen(str->ptr);

    if (len + 1 >= str->allocated)
    {
        int err = increase_size(str);
        if (err)
            return err;
    }

    str->ptr[len] = c;
    str->ptr[len + 1] = '\0';

    return EXIT_SUCCESS;
}

int ms_read_line(FILE *f, my_string_t *str)
{
    if (!f)
        return MS_FILE_ERROR;
    if (!str)
        return MS_ARGS_ERROR;

    size_t i;
    char c = '\0';
    int err;
    for (i = 0; c = fgetc(f), c != '\n' && c != EOF; i++)
        if ((err = append_symbol(str, c)))
            return err;

    if (i == 0 || is_empty(str->ptr))
        return MS_INPUT_ERROR;

    if ((err = ms_compact(str)))
        return err;

    return EXIT_SUCCESS;
}

bool is_double(char *str)
{
    if (!str || strlen(str) == 0)
        return false;

    size_t i = 0;
    if (str[0] == '+' || str[0] == '-')
        i = 1;

    int dots = 0;
    int es = 0;
    for (; str[i]; i++)
        if (str[i] == '.')
        {
            dots++;
            if (dots > 1)
                return false;
            if (es > 0)
                return false;
        }
        else if (str[i] == 'e')
        {
            es++;
            if (es > 1)
                return false;
        }
        else if (!isdigit(str[i]))
            return false;

    return true;
}

int to_double(char *str, double *res)
{
    if (!str)
        return MS_ARGS_ERROR;
    if (!res)
        return MS_ARGS_ERROR;

    if (!is_double(str))
        return MS_CONVERSION_ERROR;

    *res = atof(str);
    return EXIT_SUCCESS;
}

bool is_integer(char *str)
{
    if (!str || strlen(str) == 0)
        return false;

    size_t i = 0;
    if (str[0] == '+' || str[0] == '-')
        i = 1;

    for (; str[i]; i++)
        if (!isdigit(str[i]))
            return false;

    return true;
}

int to_integer(char *str, int *res)
{
    if (!str)
        return MS_ARGS_ERROR;
    if (!res)
        return MS_ARGS_ERROR;

    if (!is_integer(str))
        return MS_CONVERSION_ERROR;

    *res = atoi(str);
    return EXIT_SUCCESS;
}
