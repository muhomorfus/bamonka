#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "string_functions.h"
#include "return_code.h"

return_code read_line(char *str, size_t size)
{
    if (size > STR_MAX_SIZE)
        return exit_failure;

    char buffer[STR_MAX_SIZE + 1];
    buffer[0] = '\0';

    if (!fgets(buffer, size + 1, stdin) && buffer[0] != '\0')
        return exit_fgets_error;

    size_t len = strlen(buffer);
    if (len >= size)
    {
        if (str[size - 1] != '\n')
            return exit_string_overflow;
        else
            buffer[size - 1] = '\0';
    }
    else if (buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';

    strcpy(str, buffer);
    return exit_success;
}

bool has_suffix(char *str, char *suffix)
{
    size_t len_str = strlen(str);
    size_t len_suffix = strlen(suffix);

    if (len_suffix > len_str)
        return false;

    for (size_t i = 0; i < len_suffix; i++)
        if (str[len_str - 1 - i] != suffix[len_suffix - 1 - i])
            return false;

    return true;
}
