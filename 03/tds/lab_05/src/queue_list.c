#include <stdlib.h>
#include <stdbool.h>

#include "queue_list.h"
#include "error.h"

queue_list_t ql_create()
{
    queue_list_t ql = { 0 };
    return ql;
}

error_t ql_push(void *q, task_t el)
{
    queue_list_t *ql = q;

    if (!ql)
        return NEW_ERROR("передан нулевой указатель", QL_ARGS_ERROR);

    queue_node_t *node = calloc(1, sizeof(queue_node_t));
    if (!node)
        return NEW_ERROR("ошибка выделения памяти", QL_ALLOC_ERROR);

    node->content = el;
    if (ql_empty(ql))
    {
        ql->tail = node;
        ql->head = node;
    }
    else
    {
        ql->tail->next = node;
        ql->tail = ql->tail->next;
    }

    ql->len++;

    return NEW_SUCCESS();
}

error_t ql_pop(void *q, task_t *el)
{
    queue_list_t *ql = q;

    if (!ql)
        return NEW_ERROR("передан нулевой указатель", QL_ARGS_ERROR);

    if (ql_empty(ql))
        return NEW_ERROR("очередь пуста", QL_EMPTY_ERROR);

    *el = ql->head->content;
    queue_node_t *next = ql->head->next;
    free(ql->head);
    ql->head = next;
    ql->len--;
    if (ql_empty(ql))
        ql->tail = NULL;

    return NEW_SUCCESS();
}

bool ql_empty(void *q)
{
    queue_list_t *ql = q;
    return !ql || !ql->len;
}

void ql_free(void *q)
{
    task_t buf;
    while (!is_failure(ql_pop(q, &buf)));
}

error_t ql_len(void *q, size_t *len)
{
    queue_list_t *ql = q;

    if (!ql || !len)
        return NEW_ERROR("передан нулевой указатель", QL_ARGS_ERROR);

    *len = ql->len;
    return NEW_SUCCESS();
}
