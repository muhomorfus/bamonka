#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "return_code.h"
#include "file_processing.h"

bool is_positive_number(char *str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
        if (!isdigit(str[i]))
            return false;

    return true;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return exit_not_enough_args;

    if (strcmp(argv[1], "c") == 0)
    {
        if (argc < 4)
            return exit_not_enough_args;

        if (!is_positive_number(argv[2]))
            return exit_invalid_args;
        size_t number = atoi(argv[2]);
        return create_numbers_file(argv[3], number);
    }
    else if (strcmp(argv[1], "p") == 0)
    {
        if (argc < 3)
            return exit_not_enough_args;

        return print_numbers(argv[2]);
    }
    else if (strcmp(argv[1], "s") == 0)
    {
        if (argc < 3)
            return exit_not_enough_args;

        return sort_numbers(argv[2]);
    }

    return exit_invalid_args;
}