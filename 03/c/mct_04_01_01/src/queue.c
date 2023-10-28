#include <stdlib.h>
#include <stdarg.h>

#include "queue.h"

int queue_push(queue_t *q, int elem)
{
    if (!q)
        return QUEUE_ARGS_ERROR;

    return stack_push(&q->to_push, elem);
}

int queue_pop(queue_t *q, int *elem)
{
    if (!q || !elem)
        return QUEUE_ARGS_ERROR;

    if (!q->to_pop)
    {
        int err;
        int buf;
        while (!(err = stack_pop(&q->to_push, &buf)))
            if ((err = stack_push(&q->to_pop, buf)))
                return err;
        if (err != STACK_EMPTY_ERROR)
            return err;
    }

    if (!q->to_pop)
        return QUEUE_EMPTY_ERROR;

    return stack_pop(&q->to_pop, elem);
}

void queue_free(queue_t *q)
{
    stack_free(&q->to_push);
    stack_free(&q->to_pop);
}

int queue_push_many(queue_t *q, size_t n, ...)
{
    va_list args;
    int err;
    va_start(args, n);
    for (size_t i = 0; i < n; i++)
        if ((err = queue_push(q, va_arg(args, int))))
            return err;
    va_end(args);

    return EXIT_SUCCESS;
}
