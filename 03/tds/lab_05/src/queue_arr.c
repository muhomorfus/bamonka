#include <string.h>
#include <stdbool.h>

#include "queue_arr.h"
#include "error.h"
#include "oa.h"

queue_array_t qa_create()
{
    queue_array_t qa = { 0 };
    qa.head = qa.content;
    qa.tail = qa.content;
    return qa;
}

error_t qa_push(void *q, task_t el)
{
    queue_array_t *qa = q;

    if (!qa)
        return NEW_ERROR("передан нулевой указатель", QA_ARGS_ERROR);

    if (qa->len >= QA_QUEUE_SIZE)
        return NEW_ERROR("очередь переполнена", QA_OVERFLOW_ERROR);

    *(qa->tail) = el;
    qa->tail++;
    qa->len++;
    if (qa->tail >= qa->content + QA_QUEUE_SIZE)
        qa->tail = qa->content;

    return NEW_SUCCESS();
}

error_t qa_pop(void *q, task_t *el)
{
    queue_array_t *qa = q;

    if (!qa)
        return NEW_ERROR("передан нулевой указатель", QA_ARGS_ERROR);

    if (qa_empty(qa))
        return NEW_ERROR("очередь пуста", QA_EMPTY_ERROR);

    *el = *qa->head;
    qa->head++;
    qa->len--;
    if (qa->head >= qa->content + QA_QUEUE_SIZE)
        qa->head = qa->content;

    return NEW_SUCCESS();
}

bool qa_empty(void *q)
{
    queue_array_t *qa = q;
    return !qa || !qa->len;
}

error_t qa_len(void *q, size_t *len)
{
    queue_array_t *qa = q;

    if (!qa || !len)
        return NEW_ERROR("передан нулевой указатель", QA_ARGS_ERROR);

    *len = qa->len;
    return NEW_SUCCESS();
}
