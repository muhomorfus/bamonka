#ifndef QUEUE_LIST_H
#define QUEUE_LIST_H

#include <stdbool.h>

#include "mystring.h"
#include "error.h"
#include "oa.h"

#define QL_ARGS_ERROR 120
#define QL_ALLOC_ERROR 121
#define QL_EMPTY_ERROR 122

/**
 * Элемент очереди-списка.
 * content - содержимое элемента очереди;
 * next - указатель на следующий элемент очереди.
 */
typedef struct queue_node_t queue_node_t;
struct queue_node_t
{
    task_t content;
    queue_node_t *next;
};

/**
 * Очередь-список.
 * head - указатель на голову очереди (откуда удаляем);
 * tail - указатель на хвост очереди (после которого добавляем);
 * len - длина очереди.
 */
typedef struct queue_list_t
{
    queue_node_t *head;
    queue_node_t *tail;
    size_t len;
} queue_list_t;

/**
 * Создание пустой очереди.
 */
queue_list_t ql_create();

/**
 * Добавление элемента в очередь.
 * q - очередь;
 * el - добавляемый элемент.
 */
error_t ql_push(void *q, task_t el);

/**
 * Удаление элемента из очереди.
 * q - очередь;
 * el - удаленный элемент.
 */
error_t ql_pop(void *q, task_t *el);

/**
 * Проверка очереди на пустоту.
 * q - очередь.
 */
bool ql_empty(void *q);

/**
 * Освобождение очереди-списка.
 * q - очередь.
 */
void ql_free(void *q);

/**
 * Получение длины очереди.
 * q - очередь;
 * len - длина очереи.
 */
error_t ql_len(void *q, size_t *len);

#endif //QUEUE_LIST_H
