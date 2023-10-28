#ifndef STACK_LIST_H
#define STACK_LIST_H

#include <stdbool.h>

#include "mystring.h"
#include "error.h"

#define SL_ARGS_ERROR 120
#define SL_ALLOC_ERROR 121
#define SL_EMPTY_ERROR 122

/*
 * Стек-список.
 * content - содержимое элемента стека (строка);
 * next - указатель на следующий элемент стека.
 */
typedef struct stack_list_t stack_list_t;
struct stack_list_t
{
    mystring_t content;
    stack_list_t *next;
};

/*
 * Добавление элемента в стек.
 * sl - исходный стек;
 * str - добавляемая строка.
 */
error_t sl_push(stack_list_t **sl, mystring_t str);

/*
 * Удаление элемента из стека.
 * sl - исходный стек;
 * str - значение удаленной строки.
 */
error_t sl_pop(stack_list_t **sl, mystring_t str);

/*
 * Освобождение стека-списка.
 * sl - исходный стек.
 */
error_t sl_free(stack_list_t **sl);

/*
 * Проверка стека на пустоту.
 * sl - исходный стек.
 */
bool sl_empty(stack_list_t **sl);

#endif //STACK_LIST_H
