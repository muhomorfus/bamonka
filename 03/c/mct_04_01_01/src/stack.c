#include <stdlib.h>

#include "stack.h"

int stack_push(stack_t **head, int elem)
{
    if (!head)
        return STACK_ARGS_ERROR;

    stack_t *buf = calloc(1, sizeof(stack_t));
    if (!buf)
        return STACK_ALLOC_ERROR;
    buf->content = elem;

    if (!*head)
        *head = buf;
    else
    {
        buf->next = *head;
        *head = buf;
    }

    return EXIT_SUCCESS;
}

int stack_pop(stack_t **head, int *elem)
{
    if (!head || !elem)
        return STACK_ARGS_ERROR;

    if (!*head)
        return STACK_EMPTY_ERROR;

    *elem = (*head)->content;
    stack_t *buf = *head;
    *head = (*head)->next;
    free(buf);

    return EXIT_SUCCESS;
}

void stack_free(stack_t **head)
{
    int buf;
    while (!stack_pop(head, &buf));
}
