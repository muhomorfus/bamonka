#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "my_string.h"
#include "error.h"

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

error_t ms_compact(my_string_t *str)
{
    if (!str)
        return NEW_ERROR("str is null", MS_ARGS_ERROR);
    if (!str->ptr)
        return NEW_SUCCESS();

    size_t new_size = strlen(str->ptr) + 1;
    char *res = realloc(str->ptr, new_size * sizeof(char));
    if (!res)
        return NEW_ERROR("error with realloc", MS_ALLOCATION_ERROR);

    str->ptr = res;
    str->allocated = new_size;
    return NEW_SUCCESS();
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

error_t increase_size(my_string_t *str)
{
    if (!str)
        return NEW_ERROR("str is null", MS_ARGS_ERROR);

    size_t new_size = str->allocated * 2 + 2;
    char *res = realloc(str->ptr, new_size * sizeof(char));
    if (!res)
        return NEW_ERROR("error with realloc", MS_ALLOCATION_ERROR);
    str->ptr = res;
    str->allocated = new_size;

    return NEW_SUCCESS();
}

error_t append_symbol(my_string_t *str, char c)
{
    if (!str)
        return NEW_ERROR("str is null", MS_ARGS_ERROR);

    size_t len = 0;
    if (str->ptr)
        len = strlen(str->ptr);

    if (len + 1 >= str->allocated)
    {
        error_t err = increase_size(str);
        if (is_failure(err))
            return err;
    }

    str->ptr[len] = c;
    str->ptr[len + 1] = '\0';

    return NEW_SUCCESS();
}

error_t ms_read_line(FILE *f, my_string_t *str)
{
    if (!f)
        return NEW_ERROR("file is null", MS_FILE_ERROR);
    if (!str)
        return NEW_ERROR("str is null", MS_ARGS_ERROR);

    size_t i;
    char c = '\0';
    error_t err;
    for (i = 0; c = fgetc(f), c != '\n' && c != EOF; i++)
        if (is_failure(err = append_symbol(str, c)))
            return err;

    if (i == 0 || is_empty(str->ptr))
        return NEW_ERROR("line is empty", MS_INPUT_ERROR);

    if (is_failure(err = ms_compact(str)))
        return err;

    return NEW_SUCCESS();
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

error_t to_double(char *str, double *res)
{
    if (!str)
        return NEW_ERROR("str is null", MS_ARGS_ERROR);
    if (!res)
        return NEW_ERROR("res is null", MS_ARGS_ERROR);

    if (!is_double(str))
        return NEW_ERROR("str is not valid double", MS_CONVERSION_ERROR);

    *res = atof(str);
    return NEW_SUCCESS();
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

error_t to_integer(char *str, int *res)
{
    if (!str)
        return NEW_ERROR("str is null", MS_ARGS_ERROR);
    if (!res)
        return NEW_ERROR("res is null", MS_ARGS_ERROR);

    if (!is_integer(str))
        return NEW_ERROR("str is not valid integer", MS_CONVERSION_ERROR);

    *res = atoi(str);
    return NEW_SUCCESS();
}