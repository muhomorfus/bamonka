#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "my_string.h"

char *read_line(my_string_t str, FILE *f)
{
    char buf[MY_STRING_LEN + 1];
    if (fgets(buf, MY_STRING_LEN + 1, f) == NULL)
        return NULL;

    size_t last = strlen(buf);
    if (last == MY_STRING_LEN && buf[last - 1] != '\n')
        return NULL;

    if (buf[last - 1] == '\n')
        buf[last - 1] = '\0';
    else
        buf[last] = '\0';

    strcpy(str, buf);
    return str;
}

void copy_with_symbols(my_string_t str, char *symbols, my_string_t result)
{
    size_t str_len = strlen(str);

    size_t j = 0;
    for (size_t i = 0; i < str_len; i++)
        if (strchr(symbols, str[i]) != NULL)
            result[j++] = str[i];
    result[j] = '\0';
}

bool is_palindrome(char *str)
{
    size_t len = strlen(str);

    for (size_t i = 0; i < len / 2; i++)
        if (str[i] != str[len - 1 - i])
            return false;

    return true;
}


