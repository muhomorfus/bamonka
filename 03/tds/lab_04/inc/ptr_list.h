#ifndef PTR_LIST_H
#define PTR_LIST_H

#include "stack_list.h"

#define PTR_ARGS_ERROR 140
#define PTR_ALLOC_ERROR 141

/*
 * Список указателей.
 * content - указатель (содержимое элемента);
 * next - указатель на следующий элемент.
 */
typedef struct ptr_list_t ptr_list_t;
struct ptr_list_t
{
    void *content;
    ptr_list_t *next;
};

/*
 * Добавление элемента в список.
 * head - голова списка;
 * ptr - добавляемый указатель.
 */
error_t pl_add(ptr_list_t **head, void *ptr);

/*
 * Удаление указателя.
 * head - голова списка;
 * ptr - удаляемый указатель.
 */
error_t pl_delete(ptr_list_t **head, void *ptr);

/*
 * Вывод элементов списка.
 * head - голова списка.
 */
error_t pl_print(ptr_list_t **head);

/*
 * Освобождение списка.
 * head - голова списка.
 */
void pl_free(ptr_list_t **head);

#endif //PTR_LIST_H
