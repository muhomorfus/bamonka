#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "size_t_list.h"
#include "error.h"

error_t stl_append_to_element(stl_list_t **element, size_t n, stl_list_t **added)
{
    if (!added || !element || !*element)
        return NEW_ERROR("передан нулевой указатель", STL_ARGS_ERROR);

    stl_list_t *buf = calloc(1, sizeof(stl_list_t));
    if (!buf)
        return NEW_ERROR("ошибка выделения памяти", STL_ALLOC_ERROR);

    buf->value = n;
    (*element)->next = buf;
    *added = buf;

    return NEW_SUCCESS();
}

error_t stl_append_to_head(stl_list_t **head, size_t n, stl_list_t **added)
{
    if (!added || !head)
        return NEW_ERROR("передан нулевой указатель", STL_ARGS_ERROR);

    if (*added)
        return NEW_ERROR("параметр должен быть пуст", STL_ARGS_ERROR);

    *added = calloc(1, sizeof(stl_list_t));
    if (!*added)
        return NEW_ERROR("ошибка выделения памяти", STL_ALLOC_ERROR);

    (*added)->value = n;

    if (!*head)
        *head = *added;
    else
    {
        stl_list_t *element;
        for (element = *head; element->next; element = element->next);
        element->next = *added;
    }

    return NEW_SUCCESS();
}

void stl_free(stl_list_t **list)
{
    if (list && *list)
    {
        stl_list_t *next;
        for (stl_list_t *element = *list; element; element = next)
        {
            next = element->next;
            free(element);
        }
        *list = NULL;
    }
}
