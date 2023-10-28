#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "mystring.h"
#include "error.h"

bool is_empty(const char *str)
{
    for (size_t i = 0; str[i]; i++)
        if (!isspace(str[i]))
            return false;
    return true;
}

error_t f_read_line(FILE *f, mystring_t str)
{
    if (!f)
        return NEW_ERROR("Передан нулевой указатель на файл", FILE_ERROR);
    if (!str)
        return NEW_ERROR("Передан нулевой указатель на строку", ARGS_ERROR);

    char buffer[MYSTRING_SIZE + 1];

    if (!fgets(buffer, sizeof(buffer), f) || strlen(buffer) == 0)
        return NEW_ERROR("Ошибка чтения из файла", INPUT_ERROR);

    char *nlpos = strchr(buffer, '\n');
    if (!nlpos && strlen(buffer) >= MYSTRING_SIZE)
        return NEW_ERROR("Считанная строка слишком длинная", OVERFLOW_ERROR);
    if (nlpos)
        *nlpos = '\0';

    if (is_empty(buffer))
        return NEW_ERROR("Считанная строка пуста", INPUT_ERROR);

    strcpy(str, buffer);
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
    for (; str[i]; i++)
    {
        if (str[i] == '.')
        {
            dots++;
            if (dots > 1)
                return false;
        }
        else if (!isdigit(str[i]))
            return false;
    }

    return true;
}

error_t to_double(char *str, double *res)
{
    if (str == NULL)
        return NEW_ERROR("Передан нулевой указатель на строку", ARGS_ERROR);
    if (res == NULL)
        return NEW_ERROR("Передан нулевой указатель на результат", ARGS_ERROR);

    if (!is_double(str))
        return NEW_ERROR("Строка не является корректным числом", CONVERSION_ERROR);

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
    if (str == NULL)
        return NEW_ERROR("Передан нулевой указатель на строку", ARGS_ERROR);
    if (res == NULL)
        return NEW_ERROR("Передан нулевой указатель на результат", ARGS_ERROR);

    if (!is_integer(str))
        return NEW_ERROR("Строка не является корректным числом", CONVERSION_ERROR);

    *res = atoi(str);
    return NEW_SUCCESS();
}

error_t splitn(char *str, char *seps, size_t words, mystring_t *res, size_t *n)
{
    *n = 0;
    mystring_t current_string;
    size_t j = 0;
    for (size_t i = 0; i <= strlen(str); i++)
        if (strchr(seps, str[i]) || str[i] == '\0')
        {
            if (j > 0)
            {
                if (*n >= words)
                    return NEW_ERROR("Слишком много слов", OVERFLOW_ERROR);
                current_string[j] = '\0';
                strcpy(res[(*n)++], current_string);
                j = 0;
            }
        }
        else
            current_string[j++] = str[i];

    return NEW_SUCCESS();
}

void trim_spaces(char *str)
{
    size_t start;
    for (start = 0; str[start] && isspace(str[start]); start++);
    size_t finish;
    for (finish = strlen(str); finish > start && isspace(str[finish]); finish--);
    for (size_t i = start; i <= finish; i++)
        str[i - start] = str[i];

    str[finish - start] = '\0';
}

bool equals_without_size(char *str1, char *str2)
{
    if (strlen(str1) != strlen(str2))
        return false;

    for (size_t i = 0; str1[i]; i++)
        if (tolower(str1[i]) != tolower(str2[i]))
            return false;

    return true;
}

error_t align(mystring_t str, char sym, size_t len, bool center)
{
    if (len >= MYSTRING_SIZE || strlen(str) > len)
        return NEW_ERROR("Некорректный размер строки", ARGS_ERROR);

    mystring_t buf;
    size_t mod = len - strlen(str);
    size_t left = 0;
    if (center)
        left = mod / 2;

    size_t i;
    for (i = 0; i < left; i++)
        buf[i] = sym;
    for (size_t k = 0; str[k]; k++, i++)
        buf[i] = str[k];
    for (; i < len; i++)
        buf[i] = sym;
    buf[i] = '\0';
    strcpy(str, buf);

    return NEW_SUCCESS();
}
