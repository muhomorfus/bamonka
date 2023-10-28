#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "error.h"
#include "mystring.h"

error_t file_insert(char *filename, mystring_t key)
{
    if (!filename || !key)
        return NEW_ERROR("передан нулевой указатель", FILE_ARGS_ERROR);

    FILE *f = fopen(filename, "a");
    if (!f)
        return NEW_ERROR("ошибка открытия файла", FILE_OPEN_ERROR);

    if (fprintf(f, "%s\n", key) < 0)
    {
        fclose(f);
        return NEW_ERROR("ошибка записи в файл", FILE_WRITE_ERROR);
    }

    fclose(f);
    return NEW_SUCCESS();
}

error_t file_count(FILE *f, size_t *n)
{
    if (!f || !n)
        return NEW_ERROR("передан нулевой указатель", FILE_ARGS_ERROR);

    rewind(f);
    *n = 0;
    mystring_t str;
    while (fscanf(f, "%s", str) == 1)
        (*n)++;

    if (!*n)
        return NEW_ERROR("файл пуст", FILE_EMPTY_ERROR);

    rewind(f);
    return NEW_SUCCESS();
}

error_t file_remove(char *filename, mystring_t key, size_t *n_cmp)
{
    if (!filename || !key || !n_cmp)
        return NEW_ERROR("передан нулевой указатель", FILE_ARGS_ERROR);

    FILE *f = fopen(filename, "r");
    if (!f)
        return NEW_ERROR("ошибка открытия файла", FILE_OPEN_ERROR);

    error_t err;
    size_t n;
    if (is_failure(err = file_count(f, &n)))
    {
        fclose(f);
        return err;
    }

    mystring_t *keys = calloc(n, sizeof(mystring_t));
    if (!keys)
    {
        fclose(f);
        return NEW_ERROR("ошибка выделения памяти", FILE_ALLOC_ERROR);
    }

    size_t i = 0;
    while (fscanf(f, "%s", keys[i++]) == 1);
    fclose(f);
    f = fopen(filename, "w");
    if (!f)
        return NEW_ERROR("ошибка открытия файла", FILE_OPEN_ERROR);
    fclose(f);

    bool removed = false;
    for (size_t k = 0; k < n; k++)
    {
        if (!removed)
            *n_cmp += 1;

        if (strcmp(keys[k], key) != 0) {
            if (is_failure(err = file_insert(filename, keys[k])))
                return err;
        }
        else
            removed = true;
    }

    return NEW_SUCCESS();
}

error_t file_search(char *filename, mystring_t key, size_t *n_cmp)
{
    if (!filename || !key || !n_cmp)
        return NEW_ERROR("передан нулевой указатель", FILE_ARGS_ERROR);

    FILE *f = fopen(filename, "r");
    if (!f)
        return NEW_ERROR("ошибка открытия файла", FILE_OPEN_ERROR);

    mystring_t str;
    while (fscanf(f, "%s", str) == 1)
    {
        *n_cmp += 1;
        if (strcmp(str, key) == 0)
            return NEW_SUCCESS();
    }

    return NEW_ERROR("элемент не найден", FILE_NOT_FOUND_ERROR);
}
