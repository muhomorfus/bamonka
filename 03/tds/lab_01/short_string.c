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
        return NEW_ERROR("Пустой файл", SS_FILE_ERROR);
    if (!str)
        return NEW_ERROR("Нулевой указатель на строку", SS_ARGS_ERROR);

    char buffer[SHORT_STRING_SIZE + 1];

    if (!fgets(buffer, sizeof(buffer), f) || strlen(buffer) == 0)
        return NEW_ERROR("Ошибка чтения из файла", SS_INPUT_ERROR);

    char *nlpos = strchr(buffer, '\n');
    if (!nlpos && strlen(buffer) >= SHORT_STRING_SIZE)
        return NEW_ERROR("Строка слишком длинная", SS_OVERFLOW_ERROR);
    if (nlpos)
        *nlpos = '\0';

    if (is_empty(buffer))
        return NEW_ERROR("Строка пуста", SS_INPUT_ERROR);

    strcpy(str, buffer);
    return NEW_SUCCESS();
}

bool is_empty(char *str)
{
    for (size_t i = 0; str[i]; i++)
        if (!isspace(str[i]))
            return false;
    return true;
}

char *normalize_str(char *str)
{
    if (!str)
        return NULL;

    char *ptr, *ptr_next, *ptr_cur;
    for (ptr = str; *ptr && isspace(*ptr); ptr++);
    ptr_cur = ptr;
    for (ptr_next = ptr; *ptr_next; ptr_next++)
    {
        *ptr_next = tolower(*ptr_next);
        if (!isspace(*ptr_next))
        {
            *ptr_cur = *ptr_next;
            ptr_cur++;
        }
    }
    *ptr_cur = '\0';
    return ptr;
}
