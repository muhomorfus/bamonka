#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>

#define M_ARGS_ERROR 100
#define M_ALLOC_ERROR 101
#define M_INPUT_ERROR 102

typedef struct matrix_t
{
    int **ptr;
    size_t rows;
    size_t cols;
} matrix_t;

int m_new(matrix_t *m, size_t rows, size_t cols);
void m_free(matrix_t *m);
int m_find_max_pos(matrix_t *m, size_t *row, size_t *col);
int m_find_min_pos(matrix_t *m, size_t *row, size_t *col);
int m_copy(matrix_t *src, matrix_t *dst);
int m_swap_cols(matrix_t *m, size_t col1, size_t col2);
int m_create_duplicate(matrix_t *m, matrix_t *res);
int m_read_with_size(matrix_t *m);
void m_print(matrix_t *m);

#endif //MATRIX_H
