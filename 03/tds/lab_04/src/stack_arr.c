#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "stack_arr.h"
#include "mystring.h"
#include "error.h"

stack_array_t sa_create()
{
    stack_array_t sa = { 0 };
    sa.ptr = sa.content - 1;
    return sa;
}

error_t sa_push(stack_array_t *sa, mystring_t str)
{
    if (!sa || !str)
        return NEW_ERROR("переданы нулевые указатели", SA_ARGS_ERROR);


    if (sa->ptr - sa->content >= SA_STACK_SIZE - 1)
        return NEW_ERROR("переполнение стека", SA_OVERFLOW_ERROR);

    sa->ptr++;
    strcpy(*sa->ptr, str);
    return NEW_SUCCESS();
}

error_t sa_pop(stack_array_t *sa, mystring_t str)
{
    if (!sa || !str)
        return NEW_ERROR("переданы нулевые указатели", SA_ARGS_ERROR);

    if (sa->ptr < sa->content)
        return NEW_ERROR("стек пуст", SA_EMPTY_ERROR);

    strcpy(str, *(sa->ptr));
    sa->ptr--;

    return NEW_SUCCESS();
}

bool sa_empty(stack_array_t *sa)
{
    if (!sa)
        return true;

    return sa->ptr < sa->content;
}
