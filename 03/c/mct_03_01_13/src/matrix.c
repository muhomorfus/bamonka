#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "matrix.h"

int m_new(matrix_t *m, size_t rows, size_t cols)
{
    m->ptr = calloc(rows, sizeof(int *));
    if (!m->ptr)
        return M_ALLOC_ERROR;
    
    bool success = true;
    for (size_t i = 0; i < rows && success; i++)
    {
        m->ptr[i] = calloc(cols, sizeof(int));
        if (!m->ptr[i])
        {
            for (size_t k = i; k > 0; k--)
                free(m->ptr[k - 1]);
            success = false;
        }
    }
    
    if (!success)
    {
        free(m->ptr);
        return M_ALLOC_ERROR;
    }
    
    return EXIT_SUCCESS;
}

void m_free(matrix_t *m)
{
    if (m)
    {
        for (size_t i = 0; i < m->rows; i++)
            free(m->ptr[i]);
        free(m->ptr);
    }
}

int m_find_min_pos(matrix_t *m, size_t *row, size_t *col)
{
    if (!m || !m->ptr || !m->cols || !m->rows)
        return M_ARGS_ERROR;
    
    *row = *col = 0;
    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
            if (m->ptr[i][j] < m->ptr[*row][*col])
            {
                *row = i;
                *col = j;
            }
    
    return EXIT_SUCCESS;
}

int m_find_max_pos(matrix_t *m, size_t *row, size_t *col)
{
    if (!m || !m->ptr || !m->cols || !m->rows)
        return M_ARGS_ERROR;

    *row = *col = 0;
    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
            if (m->ptr[i][j] > m->ptr[*row][*col])
            {
                *row = i;
                *col = j;
            }

    return EXIT_SUCCESS;
}

int m_copy(matrix_t *src, matrix_t *dst)
{
    if (!src || !src->ptr || !src->cols || !src->rows || !dst)
        return M_ARGS_ERROR;

    int code = m_new(dst, src->rows, src->cols);
    if (code)
        return code;
    for (size_t i = 0; i < dst->rows; i++)
        for (size_t j = 0; j < dst->cols; j++)
            dst->ptr[i][j] = src->ptr[i][j];

    return EXIT_SUCCESS;
}

void swap_ints(int *a, int *b)
{
    if (a && b)
    {
        int t = *a;
        *a = *b;
        *b = t;
    }
}

int m_swap_cols(matrix_t *m, size_t col1, size_t col2)
{
    if (!m || !m->ptr || !m->cols || !m->rows)
        return M_ARGS_ERROR;

    if (col1 >= m->cols || col2 >= m->cols)
        return M_ARGS_ERROR;

    for (size_t i = 0; i < m->rows; i++)
        swap_ints(m->ptr[i] + col1, m->ptr[i] + col2);

    return EXIT_SUCCESS;
}

int m_create_duplicate(matrix_t *m, matrix_t *res)
{
    int code = m_copy(m, res);
    if (code)
        return code;

    size_t i_max, j_max;
    size_t i_min, j_min;

    code += m_find_max_pos(res, &i_max, &j_max);
    code += m_find_min_pos(res, &i_min, &j_min);
    code += m_swap_cols(res, j_min, j_max);

    if (code)
        m_free(res);
    return code;
}

int m_read_with_size(matrix_t *m)
{
    size_t rows, cols;
    if (scanf("%zu%zu", &rows, &cols) != 2 || !rows || !cols)
        return M_INPUT_ERROR;

    int code = m_new(m, rows, cols);
    if (code)
    {
        m_free(m);
        return code;
    }

    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
            if (scanf("%d", m->ptr[i] + j) != 1)
            {
                m_free(m);
                return M_INPUT_ERROR;
            }

    return EXIT_SUCCESS;
}

void m_print(matrix_t *m)
{
    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
            printf("%d ", m->ptr[i][j]);
        printf("\n");
    }
}

