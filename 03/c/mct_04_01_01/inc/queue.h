#ifndef QUEUE_H
#define QUEUE_H

#include "stack.h"

#define QUEUE_EMPTY_ERROR 20
#define QUEUE_ARGS_ERROR 21

typedef struct queue_t
{
    stack_t *to_push;
    stack_t *to_pop;
} queue_t;

int queue_push(queue_t *q, int elem);
int queue_pop(queue_t *q, int *elem);
void queue_free(queue_t *q);
int queue_push_many(queue_t *q, size_t n, ...);

#endif //QUEUE_H
