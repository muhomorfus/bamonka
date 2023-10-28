#include <stdlib.h>
#include <stdio.h>

#include "ptr_list.h"
#include "error.h"

error_t pl_add(ptr_list_t **head, void *ptr)
{
    if (!head)
        return NEW_ERROR("передан нулевой указатель", PTR_ARGS_ERROR);

    ptr_list_t *buf = calloc(1, sizeof(ptr_list_t));
    if (!buf)
        return NEW_ERROR("ошибка выделения памяти", PTR_ALLOC_ERROR);

    buf->content = ptr;
    buf->next = *head;
    *head = buf;

    return NEW_SUCCESS();
}

error_t pl_delete(ptr_list_t **head, void *ptr)
{
    if (!head)
        return NEW_ERROR("передан нулевой указатель", PTR_ARGS_ERROR);

    if (!*head)
        return NEW_SUCCESS();

    if ((*head)->content == ptr)
    {
        ptr_list_t *buf = (*head)->next;
        free(*head);
        *head = buf;
    }
    else
    {
        ptr_list_t *prev;
        for (prev = *head; prev->next && prev->next->content != ptr; prev = prev->next);
        if (prev->next)
        {
            ptr_list_t *buf = prev->next->next;
            free(prev->next);
            prev->next = buf;
        }
    }

    return NEW_SUCCESS();
}

error_t pl_print(ptr_list_t **head)
{
    if (!head)
        return NEW_ERROR("передан нулевой указатель", PTR_ARGS_ERROR);

    for (ptr_list_t *el = *head; el; el = el->next)
        printf("%p\n", el->content);

    if (!*head)
        printf("---\n");

    return NEW_SUCCESS();
}

void pl_free(ptr_list_t **head)
{
    while (*head)
    {
        ptr_list_t *buf = (*head)->next;
        free(*head);
        *head = buf;
    }
}
