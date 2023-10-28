#ifndef BST_H
#define BST_H

#include "mystring.h"

#define BST_ARGS_ERROR 400
#define BST_ALLOC_ERROR 401
#define BST_ALREADY_EXIST_ERROR 402
#define BST_EMPTY_ERROR 403
#define BST_PRINT_ERROR 404
#define BST_OPEN_ERROR 404
#define BST_READ_ERROR 405
#define BST_NOT_FOUND_ERROR 406

/**
 * Двоичное дерево поиска (ДДП).
 * key - ключ (слово);
 * left - указатель на левого потомка;
 * right - указатель на правого потомка.
 */
typedef struct bst_t bst_t;
struct bst_t
{
    mystring_t key;
    bst_t *left;
    bst_t *right;
};

/**
 * Вставка элемента в ДДП.
 * root - корень исходного дерева;
 * key - вставляемый ключ (слово).
 */
error_t bst_insert(bst_t **root, mystring_t key);

/**
 * Освобождение ДДП.
 * root - корень исходного дерева.
 */
void bst_free(bst_t **root);

/**
 * Вывод ДДП на экран в виде изображения.
 * root - корень исходного дерева.
 */
error_t bst_show(bst_t **root);

/**
 * Удаление элемента из ДДП.
 * root - корень исходного дерева;
 * key - удаляемый ключ (слово).
 */
error_t bst_remove(bst_t **root, mystring_t key, size_t *n_cmp);

/**
 * Чтение ДДП из файла.
 * root - корень исходного дерева;
 * f - файл.
 */
error_t bst_from_file(bst_t **root, FILE *f);

/**
 * Поиск элемента в ДДП.
 * root - корень исходного дерева;
 * key - искомый ключ (слово);
 * result - найденный узел.
 */
error_t bst_search(bst_t **root, mystring_t key, bst_t **result, size_t *n_cmp);

#endif //BST_H
