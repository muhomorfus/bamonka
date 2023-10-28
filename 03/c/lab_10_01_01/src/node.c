#include <stdlib.h>

#include "node.h"

void nodes_free(node_t **head)
{
    if (!head || !*head)
        return;

    node_t *next;
    for (node_t *cur = *head; cur; cur = next)
    {
        next = cur->next;
        free(cur);
    }

    *head = NULL;
}

void *pop_front(node_t **head)
{
    if (!head || !*head)
        return NULL;

    void *data = (*head)->data;
    node_t *next = (*head)->next;
    free(*head);
    *head = next;
    return data;
}

void *pop_back(node_t **head)
{
    if (!head || !*head)
        return NULL;

    if (!(*head)->next)
        return pop_front(head);

    node_t *sublast;
    for (sublast = (*head); sublast->next->next; sublast = sublast->next);
    void *data = sublast->next->data;
    node_t *next = sublast->next->next;
    free(sublast->next);
    sublast->next = next;
    return data;
}

void remove_duplicates(node_t **head, int (*comparator)(const void*, const void*))
{
    if (!head || !comparator || !*head)
        return;

    for (node_t *cur = *head; cur; cur = cur->next)
    {
        node_t *next;
        node_t *dupl;
        for (dupl = cur->next; dupl && !comparator(cur->data, dupl->data); dupl = next)
        {
            next = dupl->next;
            free(dupl);
        }
        cur->next = dupl;
    }
}

void sorted_insert(node_t **head, node_t *element, int (*comparator)(const void *, const void *))
{
    if (!head || !element || !comparator)
        return;

    if (!*head || comparator((*head)->data, element->data) > 0)
    {
        element->next = *head;
        *head = element;
        return;
    }

    node_t *prev;
    for (prev = *head; prev->next && comparator(prev->next->data, element->data) < 0; prev = prev->next);
    element->next = prev->next;
    prev->next = element;
}

node_t *sort(node_t *head, int (*comparator)(const void *, const void *))
{
    if (!head || !comparator)
        return NULL;

    node_t *result = NULL;
    for (node_t *cur = head; cur; cur = cur->next)
    {
        node_t *ins = calloc(1, sizeof(node_t));
        ins->data = cur->data;
        sorted_insert(&result, ins, comparator);
    }

    return result;
}
