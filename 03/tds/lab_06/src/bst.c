#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "bst.h"
#include "error.h"

error_t bst_insert(bst_t **root, mystring_t key)
{
    if (!root || !key)
        return NEW_ERROR("передан нулевой указатель", BST_ARGS_ERROR);

    if (!*root)
    {
        *root = calloc(1, sizeof(bst_t));
        if (!*root)
            return NEW_ERROR("ошибка выделения памяти", BST_ALLOC_ERROR);

        strcpy((*root)->key, key);
        return NEW_SUCCESS();
    }

    error_t err;
    int cmp_result = strcmp(key, (*root)->key);
    if (cmp_result < 0)
    {
        if (is_failure(err = bst_insert(&(*root)->left, key)))
            return err;
        return NEW_SUCCESS();
    }
    else if (cmp_result > 0)
    {
        if (is_failure(err = bst_insert(&(*root)->right, key)))
            return err;
        return NEW_SUCCESS();
    }

    return NEW_ERROR("элемент уже есть в дереве", BST_ALREADY_EXIST_ERROR);
}

void bst_free(bst_t **root)
{
    if (!root || !*root)
        return;

    bst_free(&(*root)->left);
    bst_free(&(*root)->right);
    free(*root);
    *root = NULL;
}

static error_t add_dot_description(bst_t **root, FILE *f)
{
    if (!root || !f)
        return NEW_ERROR("передан нулевой указатель", BST_ARGS_ERROR);

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
        return NEW_ERROR("передан нулевой указатель", BST_ARGS_ERROR);

    if (sprintf(filename, "/tmp/%ld_%d.graph.gv", time(NULL), rand()) < 0)
        return NEW_ERROR("ошибка имени файла", BST_PRINT_ERROR);

    *f = fopen(filename, "w");
    if (!*f)
        return NEW_ERROR("ошибка открытия файла", BST_OPEN_ERROR);

    return NEW_SUCCESS();
}

error_t bst_show(bst_t **root)
{
    if (!root)
        return NEW_ERROR("передан нулевой указатель", BST_ARGS_ERROR);

    if (!*root)
        return NEW_ERROR("дерево пусто", BST_EMPTY_ERROR);

    error_t err;
    FILE *f;
    mystring_t filename;
    if (is_failure(err = new_tmp_file(filename, &f)))
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
        return NEW_ERROR("ошибка записи в строку", BST_PRINT_ERROR);
    system(cmd);

    if (sprintf(cmd, "open %s.png", filename) < 0)
        return NEW_ERROR("ошибка записи в строку", BST_PRINT_ERROR);
    system(cmd);

    return NEW_SUCCESS();
}

error_t bst_remove(bst_t **root, mystring_t key, size_t *n_cmp)
{
    if (!root || !n_cmp)
        return NEW_ERROR("передан нулевой указатель", BST_ARGS_ERROR);

    if (!*root)
        return NEW_ERROR("элемент не найден", BST_NOT_FOUND_ERROR);

    int res_cmp = strcmp(key, (*root)->key);
    *n_cmp += 1;
    if (res_cmp < 0)
        return bst_remove(&(*root)->left, key, n_cmp);
    else if (res_cmp > 0)
        return bst_remove(&(*root)->right, key, n_cmp);

    bst_t *node = NULL;
    if ((*root)->left && !(*root)->right)
        node = (*root)->left;
    else if (!(*root)->left && (*root)->right)
        node = (*root)->right;

    if (node || (!(*root)->left && !(*root)->right))
    {
        free(*root);
        *root = node;
        return NEW_SUCCESS();
    }

    node = (*root)->right;
    while (node->left)
        node = node->left;

    bst_t *left = (*root)->left;
    bst_t *right = (*root)->right;
    free(*root);
    *root = right;
    node->left = left;

    return NEW_SUCCESS();
}

error_t bst_from_file(bst_t **root, FILE *f)
{
    if (!root || !f)
        return NEW_ERROR("передан нулевой указатель", BST_ARGS_ERROR);

    if (*root)
        return NEW_ERROR("дерево изначально должно быть пустым", BST_ARGS_ERROR);

    rewind(f);

    mystring_t str;
    size_t read = 0;
    while (fscanf(f, "%s", str) == 1)
    {
        error_t err;
        if (is_failure(err = bst_insert(root, str)))
            return err;

        read++;
    }

    if (!read || !feof(f))
        return NEW_ERROR("ошибка ввода дерева", BST_READ_ERROR);

    return NEW_SUCCESS();
}

error_t bst_search(bst_t **root, mystring_t key, bst_t **result, size_t *n_cmp)
{
    if (!root || !result || !key || !n_cmp)
        return NEW_ERROR("передан нулевой указатель", BST_ARGS_ERROR);

    if (*result)
        return NEW_ERROR("переменная-результат должна быть пустой", BST_ARGS_ERROR);

    if (!*root)
        return NEW_ERROR("элемент не найден", BST_NOT_FOUND_ERROR);

    int cmp_res = strcmp(key, (*root)->key);
    *n_cmp += 1;
    if (cmp_res < 0)
        return bst_search(&(*root)->left, key, result, n_cmp);
    else if (cmp_res > 0)
        return bst_search(&(*root)->right, key, result, n_cmp);

    *result = *root;
    return NEW_SUCCESS();
}
