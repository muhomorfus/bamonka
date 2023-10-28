#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <error.h>
#include <math.h>

#include "ints_matrix.h"
#include "error.h"

error_t im_allocate(ints_matrix_t *m, size_t rows, size_t cols)
{
    if (!m)
        return NEW_ERROR("m is null", IM_ARGS_ERROR);

    if (!rows || !cols)
        return NEW_ERROR("invalid size", IM_ARGS_ERROR);

    m->ptr = calloc(rows, sizeof(int *));
    if (!m->ptr)
        return NEW_ERROR("error with calloc", IM_ALLOC_ERROR);

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
        return NEW_ERROR("error with allocating rows", IM_ALLOC_ERROR);

    m->allocated_cols = cols;
    m->allocated_rows = rows;

    return NEW_SUCCESS();
}

error_t im_init(ints_matrix_t *m, size_t rows, size_t cols)
{
    error_t err;
    if (is_failure(err = im_allocate(m, rows, cols)))
        return err;

    m->rows = rows;
    m->cols = cols;

    return NEW_SUCCESS();
}

void im_free(ints_matrix_t *m)
{
    if (m && m->ptr)
    {
        for (size_t i = 0; i < m->allocated_rows; i++)
            free(m->ptr[i]);
        free(m->ptr);
        m->ptr = NULL;
        m->rows = 0;
        m->cols = 0;
        m->allocated_rows = 0;
        m->allocated_cols = 0;
    }
}

bool initialized(ints_matrix_t *m)
{
    return m && m->ptr && m->cols && m->rows && m->allocated_rows && m->allocated_cols;
}

error_t im_fill_from_file(ints_matrix_t *m, FILE *f)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
            if (fscanf(f, "%d", m->ptr[i] + j) != 1)
                return NEW_ERROR("error with reading", IM_READ_ERROR);

    return NEW_SUCCESS();
}

error_t im_fill_identity(ints_matrix_t *m)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    if (m->rows != m->cols)
        return NEW_ERROR("invalid size", IM_ARGS_ERROR);

    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
            m->ptr[i][j] = i == j;

    return NEW_SUCCESS();
}

error_t im_fill_by_matrix(ints_matrix_t *m, ints_matrix_t *src)
{
    if (!initialized(m) || !initialized(src))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    if (src->rows < m->rows || src->cols < m->cols)
        return NEW_ERROR("invalid size", IM_ARITHMETIC_ERROR);

    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
            m->ptr[i][j] = src->ptr[i][j];

    return NEW_SUCCESS();
}

error_t im_fill_multiply(ints_matrix_t *m, ints_matrix_t *a, ints_matrix_t *b)
{
    if (!initialized(m) || !initialized(a) || !initialized(b))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    if (a->cols != b->rows)
        return NEW_ERROR("invalid operands", IM_ARITHMETIC_ERROR);

    if (m->rows != a->rows || m->cols != b->cols)
        return NEW_ERROR("invalid size", IM_ARITHMETIC_ERROR);

    ints_matrix_t buf;
    error_t err;
    if (is_failure(err = im_init(&buf, m->rows, m->cols)))
        return err;

    for (size_t i = 0; i < buf.rows; i++)
        for (size_t j = 0; j < buf.cols; j++)
            for (size_t k = 0; k < a->cols; k++)
                buf.ptr[i][j] += a->ptr[i][k] * b->ptr[k][j];

    err = im_fill_by_matrix(m, &buf);
    im_free(&buf);
    return err;
}

error_t im_print(ints_matrix_t *m, FILE *f)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
            if (fprintf(f, "%d ", m->ptr[i][j]) < 0)
                return NEW_ERROR("error with printing", IM_PRINT_ERROR);
        fprintf(f, "\n");
    }

    return NEW_SUCCESS();
}

error_t change_allocated_cols(ints_matrix_t *m, size_t new_cols)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    if (!new_cols)
        return NEW_ERROR("invalid number of cols", IM_ARGS_ERROR);

    for (size_t i = 0; i < m->allocated_rows; i++)
    {
        int *buf = realloc(m->ptr[i], new_cols * sizeof(int));
        if (!buf)
            return NEW_ERROR("error with realloc", IM_ALLOC_ERROR);
        m->ptr[i] = buf;
    }

    for (size_t i = 0; i < m->allocated_rows; i++)
        for (size_t j = m->allocated_cols; j < new_cols; j++)
            m->ptr[i][j] = 0;

    m->allocated_cols = new_cols;
    if (new_cols < m->cols)
        m->cols = new_cols;

    return NEW_SUCCESS();
}

error_t change_allocated_rows(ints_matrix_t *m, size_t new_rows)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    if (!new_rows)
        return NEW_ERROR("invalid number of rows", IM_ARGS_ERROR);

    for (size_t i = new_rows; i < m->allocated_rows; i++)
        free(m->ptr[i]);

    int **buf = realloc(m->ptr, new_rows * sizeof(int *));
    if (!buf)
        return NEW_ERROR("error with realloc", IM_ALLOC_ERROR);
    m->ptr = buf;

    for (size_t i = m->allocated_rows; i < new_rows; i++)
    {
        m->ptr[i] = calloc(m->allocated_cols, sizeof(int));
        if (!m->ptr[i])
            return NEW_ERROR("error with allocating row", IM_ALLOC_ERROR);
        m->allocated_rows++;
    }

    m->allocated_rows = new_rows;
    if (new_rows < m->rows)
        m->rows = new_rows;

    return NEW_SUCCESS();
}

error_t im_delete_col(ints_matrix_t *m, size_t col)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    if (col >= m->cols)
        return NEW_ERROR("invalid number of column", IM_OUT_OF_RANGE_ERROR);

    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = col; j < m->cols - 1; j++)
            m->ptr[i][j] = m->ptr[i][j + 1];

    m->cols--;

    return NEW_SUCCESS();
}

error_t im_delete_row(ints_matrix_t *m, size_t row)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    if (row >= m->rows)
        return NEW_ERROR("invalid number of row", IM_OUT_OF_RANGE_ERROR);

    int *buf = m->ptr[row];
    for (size_t i = row; i < m->rows - 1; i++)
    {
        m->ptr[i] = m->ptr[i + 1];
    }

    m->ptr[m->rows - 1] = buf;
    m->rows--;

    return NEW_SUCCESS();
}

error_t im_average_of_column(ints_matrix_t *m, size_t col, double *avg)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    if (col >= m->cols)
        return NEW_ERROR("invalid number of column", IM_OUT_OF_RANGE_ERROR);

    if (!avg)
        return NEW_ERROR("null pointer", IM_ARGS_ERROR);

    *avg = 0.0;
    for (size_t i = 0; i < m->rows; i++)
        *avg += m->ptr[i][col];
    *avg /= (double) m->rows;

    return NEW_SUCCESS();
}

error_t im_max_of_row(ints_matrix_t *m, size_t row, int *maximum)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    if (row >= m->rows)
        return NEW_ERROR("invalid number of row", IM_OUT_OF_RANGE_ERROR);

    if (!maximum)
        return NEW_ERROR("null pointer", IM_ARGS_ERROR);

    *maximum = m->ptr[row][0];
    for (size_t j = 0; j < m->cols; j++)
        if (m->ptr[row][j] > *maximum)
            *maximum = m->ptr[row][j];

    return NEW_SUCCESS();
}

error_t im_max_element_pos(ints_matrix_t *m, size_t *imax, size_t *jmax)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    if (!imax || !jmax)
        return NEW_ERROR("null pointer", IM_ARGS_ERROR);

    *imax = 0;
    *jmax = 0;

    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
            if (m->ptr[i][j] > m->ptr[*imax][*jmax])
            {
                *imax = i;
                *jmax = j;
            }

    return NEW_SUCCESS();
}

error_t max_element_row(ints_matrix_t *m, size_t *imax)
{
    size_t buf;
    return im_max_element_pos(m, imax, &buf);
}

error_t max_element_col(ints_matrix_t *m, size_t *jmax)
{
    size_t buf;
    return im_max_element_pos(m, &buf, jmax);
}

error_t im_make_square(ints_matrix_t *m)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    error_t err;
    error_t (*max_func)(ints_matrix_t *, size_t *);
    error_t (*del_func)(ints_matrix_t *, size_t);
    if (m->cols > m->rows)
    {
        max_func = max_element_col;
        del_func = im_delete_col;
    }
    else
    {
        max_func = max_element_row;
        del_func = im_delete_row;
    }

    size_t ind;
    while (m->rows != m->cols)
    {
        if (is_failure(err = max_func(m, &ind)))
            return err;
        if (is_failure(err = del_func(m, ind)))
            return err;
    }

    return NEW_SUCCESS();
}

error_t im_increase_to_size(ints_matrix_t *m, size_t rows, size_t cols)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", IM_ARGS_ERROR);

    if (rows < m->rows || cols < m->cols)
        return NEW_ERROR("invalid sizes", IM_ARGS_ERROR);

    error_t err;
    if (is_failure(err = change_allocated_cols(m, cols)))
        return err;
    if (is_failure(err = change_allocated_rows(m, rows)))
        return err;
    while (m->rows < rows)
    {
        for (size_t j = 0; j < m->cols; j++)
        {
            double avg;
            if (is_failure(err = im_average_of_column(m, j, &avg)))
                return err;
            m->ptr[m->rows][j] = floor(avg);
        }
        m->rows++;
    }

    while (m->cols < cols)
    {
        for (size_t i = 0; i < m->rows; i++)
        {
            int maximum;
            if (is_failure(err = im_max_of_row(m, i, &maximum)))
                return err;
            m->ptr[i][m->cols] = maximum;
        }
        m->cols++;
    }

    return NEW_SUCCESS();
}
