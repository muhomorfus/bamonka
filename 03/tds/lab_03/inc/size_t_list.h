#ifndef SIZE_T_LIST_H
#define SIZE_T_LIST_H

#define STL_ARGS_ERROR 141
#define STL_ALLOC_ERROR 142

#include <stdlib.h>

#include "error.h"

/*
 * Односвязный список.
 * value - значение элемента списка;
 * next - указатель на следующий элемент списка.
 */
typedef struct stl_list_t stl_list_t;
struct stl_list_t
{
    size_t value;
    stl_list_t *next;
};

/*
 * Добавление элемента к последнему элементу списка.
 * element - последний элемент списка;
 * n - значение нового элемента;
 * added - указатель на добавленный элементю.
 */
error_t stl_append_to_element(stl_list_t **element, size_t n, stl_list_t **added);

/*
 * Добавление элемента в конец списка.
 * head - указатель на голову списка;
 * n - значение нового элемента;
 * added - указатель на добавленный элементю.
 */
error_t stl_append_to_head(stl_list_t **head, size_t n, stl_list_t **added);

/*
 * Очистка списка.
 * list - указатель на голову списка.
 */
void stl_free(stl_list_t **list);

#endif //SIZE_T_LIST_H
