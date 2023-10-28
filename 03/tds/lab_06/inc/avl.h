#ifndef AVL_H
#define AVL_H

#include "mystring.h"

#define AVL_ARGS_ERROR 500
#define AVL_ALLOC_ERROR 501
#define AVL_ALREADY_EXIST_ERROR 502
#define AVL_EMPTY_ERROR 503
#define AVL_PRINT_ERROR 504
#define AVL_OPEN_ERROR 504
#define AVL_HEIGHT_ERROR 505
#define AVL_READ_ERROR 506
#define AVL_NOT_FOUND_ERROR 507

/**
 * АВЛ-дерево.
 * key - ключ (слово);
 * height - высота текущего узла;
 * left - указатель на левого потомка;
 * right - указатель на правого потомка.
 */
typedef struct avl_t avl_t;
struct avl_t
{
    mystring_t key;
    int height;
    avl_t *left;
    avl_t *right;
};

/**
 * Вставка элемента в АВЛ-дерево.
 * root - корень исходного дерева;
 * key - вставляемый ключ (слово).
 */
error_t avl_insert(avl_t **root, mystring_t key);

/**
 * Освобождение АВЛ-дерева.
 * root - корень исходного дерева.
 */
void avl_free(avl_t **root);

/**
 * Вывод АВЛ-дерева на экран в виде изображения.
 * root - корень исходного дерева.
 */
error_t avl_show(avl_t **root);

/**
 * Удаление элемента из АВЛ-дерева.
 * root - корень исходного дерева;
 * key - удаляемый ключ (слово).
 */
error_t avl_remove(avl_t **root, mystring_t key, size_t *n_cmp);

/**
 * Чтение АВЛ-дерева из файла.
 * root - корень исходного дерева;
 * f - файл.
 */
error_t avl_from_file(avl_t **root, FILE *f);

/**
 * Поиск элемента в АВЛ-дереве.
 * root - корень исходного дерева;
 * key - искомый ключ (слово);
 * result - найденный узел.
 */
error_t avl_search(avl_t **root, mystring_t key, avl_t **result, size_t *n_cmp);

#endif //AVL_H
