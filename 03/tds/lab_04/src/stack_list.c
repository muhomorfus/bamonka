#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "stack_list.h"
#include "mystring.h"
#include "error.h"

error_t sl_push(stack_list_t **sl, mystring_t str)
{
    if (!sl || !str)
        return NEW_ERROR("переданы нулевые указатели", SL_ARGS_ERROR);

    stack_list_t *buf = calloc(1, sizeof(stack_list_t));
    if (!buf)
        return NEW_ERROR("ошибка выделения памяти", SL_ALLOC_ERROR);

    strcpy(buf->content, str);
    buf->next = *sl;
    *sl = buf;

    return NEW_SUCCESS();
}

error_t sl_pop(stack_list_t **sl, mystring_t str)
{
    if (!sl || !str)
        return NEW_ERROR("переданы нулевые указатели", SL_ARGS_ERROR);

    if (!*sl)
        return NEW_ERROR("стек пуст", SL_EMPTY_ERROR);

    strcpy(str, (*sl)->content);
    stack_list_t *next = (*sl)->next;
    free(*sl);
    *sl = next;

    return NEW_SUCCESS();
}

error_t sl_free(stack_list_t **sl)
{
    mystring_t str;
    error_t err = NEW_SUCCESS();
    while (!is_failure(err = sl_pop(sl, str)));
    if (err.code != SL_EMPTY_ERROR)
        return err;

    *sl = NULL;
    return NEW_SUCCESS();
}

bool sl_empty(stack_list_t **sl)
{
    return !sl || !*sl;
}
