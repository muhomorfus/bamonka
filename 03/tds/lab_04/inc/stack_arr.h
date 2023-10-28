#ifndef STACK_ARR_H
#define STACK_ARR_H

#include <stdbool.h>

#include "mystring.h"
#include "error.h"

/*
 * Максимальное количество элементов стека-массива.
 */
#define SA_STACK_SIZE 5000

#define SA_OVERFLOW_ERROR 100
#define SA_EMPTY_ERROR 101
#define SA_ARGS_ERROR 102

/*
 * Стек-массив.
 * content - содержимое стека;
 * ptr - указатель на место, в которое вставлять очередной
 *       элемент стека.
 */
typedef struct stack_arr_t
{
    mystring_t content[SA_STACK_SIZE];
    mystring_t *ptr;
} stack_array_t;

/*
 * Создание пустого стека.
 */
stack_array_t sa_create();

/*
 * Добавление элемента в стек.
 * sa - исходный стек;
 * str - добавляемая строка.
 */
error_t sa_push(stack_array_t *sa, mystring_t str);

/*
 * Удаление элемента из стека.
 * sa - исходный стек;
 * str - значение удаленной строки.
 */
error_t sa_pop(stack_array_t *sa, mystring_t str);


/*
 * Проверка стека на пустоту.
 * sa - исходный стек.
 */
bool sa_empty(stack_array_t *sa);

#endif //STACK_ARR_H
