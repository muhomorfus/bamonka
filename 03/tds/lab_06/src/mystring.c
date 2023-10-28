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

error_t ms_read_line(FILE *f, mystring_t str)
{
    if (!f)
        return NEW_ERROR("передан нулевой указатель на файл", FILE_ERROR);
    if (!str)
        return NEW_ERROR("передан нулевой указатель на строку", ARGS_ERROR);

    char buffer[MYSTRING_SIZE + 1];

    if (!fgets(buffer, sizeof(buffer), f) || strlen(buffer) == 0)
        return NEW_ERROR("ошибка чтения из файла", INPUT_ERROR);

    char *nlpos = strchr(buffer, '\n');
    if (!nlpos && strlen(buffer) >= MYSTRING_SIZE)
        return NEW_ERROR("считанная строка слишком длинная", OVERFLOW_ERROR);
    if (nlpos)
        *nlpos = '\0';

    if (is_empty(buffer))
        return NEW_ERROR("считанная строка пуста", INPUT_ERROR);

    strcpy(str, buffer);
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
        return NEW_ERROR("передан нулевой указатель на строку", ARGS_ERROR);
    if (res == NULL)
        return NEW_ERROR("передан нулевой указатель на результат", ARGS_ERROR);

    if (!is_integer(str))
        return NEW_ERROR("строка не является корректным числом", CONVERSION_ERROR);

    *res = atoi(str);
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

error_t ms_read_int_line(FILE *f, int *n)
{
    if (!n)
        return NEW_ERROR("передан пустой указатель", ARGS_ERROR);

    mystring_t str;
    error_t err = ms_read_line(f, str);
    if (is_failure(err))
        return err;

    trim_spaces(str);
    return to_integer(str, n);
}

error_t ms_read_word(FILE *f, mystring_t str)
{
    if (!str || !f)
        return NEW_ERROR("передан пустой указатель", ARGS_ERROR);

    char buffer[MYSTRING_SIZE + 1];

    int read = fscanf(f, "%257s", buffer);
    fflush(f);
    if (read != 1)
        return NEW_ERROR("ошибка ввода слова", INPUT_ERROR);

    if (strlen(buffer) >= MYSTRING_SIZE)
        return NEW_ERROR("введенная строка слишком длинная", INPUT_ERROR);

    strcpy(str, buffer);
    return NEW_SUCCESS();
}

