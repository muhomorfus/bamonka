#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "avl.h"
#include "error.h"

static int max(int a, int b)
{
    if (a > b)
        return a;

    return b;
}

static int height(avl_t **root)
{
    if (!root)
        return INT_MIN;
    if (!*root)
        return 0;
    return (*root)->height;
}

static int balance_factor(avl_t **root)
{
    if (!root)
        return INT_MIN;

    return height(&(*root)->right) - height(&(*root)->left);
}

static int fill_height(avl_t **root)
{
    if (!root)
        return INT_MIN;

    if (!*root)
        return 0;

    (*root)->height = max(fill_height(&(*root)->left), fill_height(&(*root)->right)) + 1;
    return (*root)->height;
}

static error_t fill_heights(avl_t **root)
{
    int code = fill_height(root);
    if (code == INT_MIN)
        return NEW_ERROR("ошибка расчета высот", AVL_HEIGHT_ERROR);

    return NEW_SUCCESS();
}

void avl_free(avl_t **root)
{
    if (!root || !*root)
        return;

    avl_free(&(*root)->left);
    avl_free(&(*root)->right);
    free(*root);
    *root = NULL;
}

static error_t add_dot_description(avl_t **root, FILE *f)
{
    if (!root || !f)
        return NEW_ERROR("передан нулевой указатель", AVL_ARGS_ERROR);

    if (!*root)
        return NEW_SUCCESS();

    if ((*root)->left)
        fprintf(f, "\"%s\" -> \"%s\" [label=\"L\", color=\"black\"];\n", (*root)->key, (*root)->left->key);

    if ((*root)->right)
        fprintf(f, "\"%s\" -> \"%s\" [label=\"R\", color=\"black\"];\n", (*root)->key, (*root)->right->key);

    error_t err;
    if (is_failure(err = add_dot_description(&(*root)->left, f)))
        return err;

    return add_dot_description(&(*root)->right, f);
}

static error_t new_tmp_file(mystring_t filename, FILE **f)
{
    if (!f || !filename)
        return NEW_ERROR("передан нулевой указатель", AVL_ARGS_ERROR);

    if (sprintf(filename, "/tmp/%ld_%d.avl.graph.gv", time(NULL), rand()) < 0)
        return NEW_ERROR("ошибка имени файла", AVL_PRINT_ERROR);

    *f = fopen(filename, "w");
    if (!*f)
        return NEW_ERROR("ошибка открытия файла", AVL_OPEN_ERROR);

    return NEW_SUCCESS();
}

error_t avl_show(avl_t **root)
{
    if (!root)
        return NEW_ERROR("передан нулевой указатель", AVL_ARGS_ERROR);

    if (!*root)
        return NEW_ERROR("дерево пусто", AVL_EMPTY_ERROR);

    error_t err;
    FILE *f;
    mystring_t filename;
    if (is_failure(err = new_tmp_file(filename, &f)))
        return err;

    if (is_failure(err = fill_heights(root)))
        return err;

    fprintf(f, "digraph some_graph {\n");
    fprintf(f, "node [shape=box, style=\"rounded\", fontname=\"Jetbrains Mono\", fontsize=10];\n");
    fprintf(f, "edge [fontname=\"Jetbrains Mono\", fontsize=10];\n");
    if (is_failure(err = add_dot_description(root, f)))
    {
        fclose(f);
        return err;
    }
    fprintf(f, "}\n");
    fclose(f);

    mystring_t cmd;
    if (sprintf(cmd, "/opt/homebrew/bin/dot -Tpng %s -o %s.png", filename, filename) < 0)
        return NEW_ERROR("ошибка записи в строку", AVL_PRINT_ERROR);
    system(cmd);

    if (sprintf(cmd, "open %s.png", filename) < 0)
        return NEW_ERROR("ошибка записи в строку", AVL_PRINT_ERROR);
    system(cmd);

    return NEW_SUCCESS();
}

static error_t fix_height(avl_t **root)
{
    if (!root)
        return NEW_ERROR("передан нулевой указатель", AVL_ARGS_ERROR);

    int height_left = height(&(*root)->left);
    int height_right = height(&(*root)->right);
    if (height_left > height_right)
        (*root)->height = height_left;
    else
        (*root)->height = height_right;
    (*root)->height++;

    return NEW_SUCCESS();
}

static error_t rotate_left(avl_t **root)
{
    if (!root)
        return NEW_ERROR("передан нулевой указатель", AVL_ARGS_ERROR);

    if (!*root)
        return NEW_SUCCESS();

    avl_t *new_root = (*root)->right;
    (*root)->right = new_root->left;
    new_root->left = *root;

    error_t err;
    if (is_failure(err = fix_height(root)))
        return err;
    if (is_failure(err = fix_height(&new_root)))
        return err;
    *root = new_root;

    return NEW_SUCCESS();
}

static error_t rotate_right(avl_t **root)
{
    if (!root)
        return NEW_ERROR("передан нулевой указатель", AVL_ARGS_ERROR);

    if (!*root)
        return NEW_SUCCESS();

    avl_t *new_root = (*root)->left;
    (*root)->left = new_root->right;
    new_root->right = *root;

    error_t err;
    if (is_failure(err = fix_height(root)))
        return err;
    if (is_failure(err = fix_height(&new_root)))
        return err;
    *root = new_root;

    return NEW_SUCCESS();
}

static error_t balance_one(avl_t **root)
{
    if (!root)
        return NEW_ERROR("передан нулевой указатель", AVL_ARGS_ERROR);

    if (!*root)
        return NEW_SUCCESS();

    error_t err;
    if (is_failure(err = fill_heights(root)))
        return err;

    if (is_failure(err = fix_height(root)))
        return err;

    if (balance_factor(root) == 2)
    {
        if (balance_factor(&(*root)->right) < 0)
            if (is_failure(err = rotate_right(&(*root)->right)))
                return err;
        return rotate_left(root);
    }

    if (balance_factor(root) == -2)
    {
        if (balance_factor(&(*root)->left) > 0)
            if (is_failure(err = rotate_left(&(*root)->left)))
                return err;
        return rotate_right(root);
    }

    return NEW_SUCCESS();
}

error_t avl_insert(avl_t **root, mystring_t key)
{
    if (!root || !key)
        return NEW_ERROR("передан нулевой указатель", AVL_ARGS_ERROR);

    error_t err;
    if (!*root)
    {
        *root = calloc(1, sizeof(avl_t));
        if (!*root)
            return NEW_ERROR("ошибка выделения памяти", AVL_ALLOC_ERROR);

        (*root)->height = 1;
        strcpy((*root)->key, key);
        return NEW_SUCCESS();
    }

    int cmp_result = strcmp(key, (*root)->key);
    if (cmp_result < 0)
    {
        if (is_failure(err = avl_insert(&(*root)->left, key)))
            return err;

        if (is_failure(err = balance_one(root)))
            return err;

        return NEW_SUCCESS();
    }
    else if (cmp_result > 0)
    {
        if (is_failure(err = avl_insert(&(*root)->right, key)))
            return err;

        if (is_failure(err = balance_one(root)))
            return err;

        return NEW_SUCCESS();
    }

    return NEW_ERROR("элемент уже есть в дереве", AVL_ALREADY_EXIST_ERROR);
}

error_t avl_remove(avl_t **root, mystring_t key, size_t *n_cmp)
{
    if (!root || !n_cmp)
        return NEW_ERROR("передан нулевой указатель", AVL_ARGS_ERROR);

    if (!*root)
        return NEW_ERROR("элемент не найден", AVL_NOT_FOUND_ERROR);

    int res_cmp = strcmp(key, (*root)->key);
    *n_cmp += 1;
    if (res_cmp < 0)
        return avl_remove(&(*root)->left, key, n_cmp);
    else if (res_cmp > 0)
        return avl_remove(&(*root)->right, key, n_cmp);

    avl_t *node = NULL;
    if ((*root)->left && !(*root)->right)
        node = (*root)->left;
    else if (!(*root)->left && (*root)->right)
        node = (*root)->right;

    if (node || (!(*root)->left && !(*root)->right))
    {
        free(*root);
        *root = node;
        return balance_one(root);
    }

    node = (*root)->right;
    while (node->left)
        node = node->left;

    avl_t *left = (*root)->left;
    avl_t *right = (*root)->right;
    free(*root);
    *root = right;
    node->left = left;

    return balance_one(root);
}

error_t avl_from_file(avl_t **root, FILE *f)
{
    if (!root || !f)
        return NEW_ERROR("передан нулевой указатель", AVL_ARGS_ERROR);

    if (*root)
        return NEW_ERROR("дерево изначально должно быть пустым", AVL_ARGS_ERROR);

    rewind(f);

    mystring_t str;
    size_t read = 0;
    while (fscanf(f, "%s", str) == 1)
    {
        error_t err;
        if (is_failure(err = avl_insert(root, str)))
            return err;

        read++;
    }

    if (!read || !feof(f))
        return NEW_ERROR("ошибка ввода сбалансированного дерева", AVL_READ_ERROR);

    return NEW_SUCCESS();
}

error_t avl_search(avl_t **root, mystring_t key, avl_t **result, size_t *n_cmp)
{
    if (!root || !result || !key || !n_cmp)
        return NEW_ERROR("передан нулевой указатель", AVL_ARGS_ERROR);

    if (*result)
        return NEW_ERROR("переменная-результат должна быть пустой", AVL_ARGS_ERROR);

    if (!*root)
        return NEW_ERROR("элемент не найден", AVL_NOT_FOUND_ERROR);

    int cmp_res = strcmp(key, (*root)->key);
    *n_cmp += 1;
    if (cmp_res < 0)
        return avl_search(&(*root)->left, key, result, n_cmp);
    else if (cmp_res > 0)
        return avl_search(&(*root)->right, key, result, n_cmp);

    *result = *root;
    return NEW_SUCCESS();
}
