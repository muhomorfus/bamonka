#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "short_string.h"
#include "error.h"

error_t f_read_line(FILE *f, short_string_t str)
{
    if (!f)
        return new_error("file is null", FILE_ERROR);
    if (!str)
        return new_error("str is null", ARGS_ERROR);

    char buffer[SHORT_STRING_SIZE + 1];

    if (!fgets(buffer, sizeof(buffer), f) || strlen(buffer) == 0)
        return new_error("line reading error from f", INPUT_ERROR);

    char *nlpos = strchr(buffer, '\n');
    if (!nlpos && strlen(buffer) >= SHORT_STRING_SIZE)
        return new_error("line is too long", OVERFLOW_ERROR);
    if (nlpos)
        *nlpos = '\0';

    if (is_empty(buffer))
        return new_error("line is empty", INPUT_ERROR);

    strcpy(str, buffer);
    return new_success();
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
    if (str == NULL)
        return new_error("str is null", ARGS_ERROR);
    if (res == NULL)
        return new_error("res is null", ARGS_ERROR);

    if (!is_double(str))
        return new_error("str is not valid double", CONVERSION_ERROR);

    *res = atof(str);
    return new_success();
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
    if (str == NULL)
        return new_error("str is null", ARGS_ERROR);
    if (res == NULL)
        return new_error("res is null", ARGS_ERROR);

    if (!is_integer(str))
        return new_error("str is not valid integer", CONVERSION_ERROR);

    *res = atoi(str);
    return new_success();
}

bool is_empty(char *str)
{
    for (size_t i = 0; str[i]; i++)
        if (!isspace(str[i]))
            return false;
    return true;
}
