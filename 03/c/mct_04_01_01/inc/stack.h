#ifndef STACK_H
#define STACK_H

#define STACK_ALLOC_ERROR 10
#define STACK_ARGS_ERROR 11
#define STACK_EMPTY_ERROR 12

typedef struct stack_t stack_t;
struct stack_t
{
    int content;
    stack_t *next;
};

int stack_push(stack_t **head, int elem);
int stack_pop(stack_t **head, int *elem);
void stack_free(stack_t **head);

#endif //STACK_H
