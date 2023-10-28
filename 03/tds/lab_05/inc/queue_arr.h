#ifndef QUEUE_ARR_H
#define QUEUE_ARR_H

#include <stdbool.h>

#include "error.h"
#include "oa.h"

/**
 * Максимальное количество элементов очереди-массива.
 */
#define QA_QUEUE_SIZE 20000

#define QA_OVERFLOW_ERROR 100
#define QA_EMPTY_ERROR 101
#define QA_ARGS_ERROR 102

/**
 * Очередь-массив.
 * content - содержимое очереди;
 * head - указатель на голову очереди, то есть на адрес
 *        удаляемого элемента;
 * tail - указатель на хвост очереди, то есть на адрес
 *        куда будет вставляться очередной элемент;
 * len - длина очереди.
 */
typedef struct queue_array_t
{
    task_t content[QA_QUEUE_SIZE];
    task_t *head;
    task_t *tail;
    size_t len;
} queue_array_t;

/**
 * Создание пустой очереди.
 */
queue_array_t qa_create();

/**
 * Добавление элемента в очередь.
 * q - очередь;
 * el - добавляемый элемент.
 */
error_t qa_push(void *q, task_t el);

/**
 * Удаление элемента из очереди.
 * q - очередь;
 * el - удаленный элемент.
 */
error_t qa_pop(void *q, task_t *el);

/**
 * Проверка очереди на пустоту.
 * q - очередь.
 */
bool qa_empty(void *q);

/**
 * Получение длины очереди.
 * q - очередь;
 * len - длина очереи.
 */
error_t qa_len(void *q, size_t *len);

#endif //QUEUE_ARR_H
