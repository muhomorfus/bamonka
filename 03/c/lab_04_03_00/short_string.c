#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include "short_string.h"

char *read_line(short_string_t str)
{
    char buffer[SHORT_STRING_SIZE + 1];
    buffer[0] = '\0';

    if (!fgets(buffer, sizeof(buffer), stdin) && buffer[0] != '\0')
        return NULL;

    size_t len = strlen(buffer);
    if (len == SHORT_STRING_SIZE && buffer[len - 1] != '\n')
        return NULL;
    else if (buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';

    strcpy(str, buffer);
    return str;
}

bool is_correct_positive(char *str)
{
    size_t len = strlen(str);
    if (len == 0)
        return false;

    for (size_t i = 0; i < len; i++)
        if (!isdigit(str[i]))
            return false;

    return true;
}

size_t split(char *str, char *seps, short_string_t *strs)
{
    size_t current_string_index = 0;

    short_string_t current_string;
    size_t j = 0;
    for (size_t i = 0; i <= strlen(str); i++)
        if (strchr(seps, str[i]) || str[i] == '\0')
        {
            if (j > 0)
            {
                current_string[j] = '\0';
                strcpy(strs[current_string_index++], current_string);
                j = 0;
            }
        }
        else
        {
            if (j >= WORD_SIZE)
                return 0;
            current_string[j++] = str[i];
        }

    return current_string_index;
}

size_t split_by_spaces(char *str, short_string_t *strs)
{
    size_t current_string_index = 0;

    short_string_t current_string;
    size_t j = 0;
    for (size_t i = 0; i <= strlen(str); i++)
        if (isspace(str[i]) || str[i] == '\0')
        {
            if (j > 0)
            {
                current_string[j] = '\0';
                strcpy(strs[current_string_index++], current_string);
                j = 0;
            }
        }
        else
        {
            if (j >= WORD_SIZE)
                return 0;
            current_string[j++] = str[i];
        }

    return current_string_index;
}

size_t strtok_split(char *str, char *seps, short_string_t *strs)
{
    size_t current_string_index = 0;
    char *string = strtok(str, seps);
    while (string != NULL)
    {
        if (strlen(string) > WORD_SIZE)
            return 0;

        strcpy(strs[current_string_index++], string);
        string = strtok(NULL, seps);
    }

    return current_string_index;
}

bool includes(short_string_t *strs, size_t n, short_string_t str)
{
    for (size_t i = 0; i < n; i++)
        if (strcmp(str, strs[i]) == 0)
            return true;

    return false;
}

size_t join(short_string_t *strs, size_t n, char *sep, short_string_t str)
{
    str[0] = '\0';
    bool can_join = strlen(strs[0]) + strlen(str) <= SHORT_STRING_SIZE;

    for (size_t i = 0; i < n && can_join; i++)
    {
        strcat(str, strs[i]);
        if (i != n - 1)
            strcat(str, sep);
        can_join = strlen(strs[i]) + strlen(str) <= SHORT_STRING_SIZE;
    }

    return strlen(str);
}

void swap(short_string_t s1, short_string_t s2)
{
    short_string_t t;
    strcpy(t, s1);
    strcpy(s1, s2);
    strcpy(s2, t);
}

void reverse(short_string_t *strs, size_t n)
{
    for (size_t i = 0; i < n / 2; i++)
        swap(strs[i], strs[n - i - 1]);
}

void to_lower(char *str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
        str[i] = tolower(str[i]);
}

size_t delete_duplicates_of_first(char *str)
{
    size_t len = strlen(str);
    if (len == 0)
        return len;

    char first = str[0];

    for (size_t i = 1; i < len; i++)
        if (str[i] == first)
            len = delete_char(str, i--);

    return len;
}

size_t delete_char(char *str, size_t index)
{
    size_t len = strlen(str);
    if (index >= len)
        return len;

    for (size_t i = index; i < len; i++)
        str[i] = str[i + 1];

    return len - 1;
}

size_t delete_str(short_string_t *strs, size_t n, size_t index)
{
    if (n == 0)
        return 0;

    if (index >= n)
        return n;

    for (size_t i = index; i < n - 1; i++)
        strcpy(strs[i], strs[i + 1]);

    return n - 1;
}

size_t delete_occurrences(short_string_t *strs, size_t n, short_string_t str)
{
    for (size_t i = 0; i < n; i++)
        if (strcmp(strs[i], str) == 0)
            n = delete_str(strs, n, i--);

    return n;
}

size_t delete_duplicates(short_string_t *strs, size_t n)
{
    for (size_t i = 0; i < n; i++)
        if (includes(strs, i, strs[i]))
            n = delete_str(strs, n, i--);

    return n;
}

