#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <error.h>
#include <math.h>

#include "doubles_matrix.h"
#include "error.h"

error_t dm_allocate(doubles_matrix_t *m, size_t rows, size_t cols)
{
    if (!m)
        return NEW_ERROR("m is null", DM_ARGS_ERROR);

    if (!rows || !cols)
        return NEW_ERROR("invalid size", DM_ARGS_ERROR);

    m->ptr = calloc(rows, sizeof(double *));
    if (!m->ptr)
        return NEW_ERROR("error with calloc", DM_ALLOC_ERROR);

    bool success = true;
    for (size_t i = 0; i < rows && success; i++)
    {
        m->ptr[i] = calloc(cols, sizeof(double));
        if (!m->ptr[i])
        {
            for (size_t k = i; k > 0; k--)
                free(m->ptr[k - 1]);
            success = false;
        }
    }

    if (!success)
        return NEW_ERROR("error with allocating rows", DM_ALLOC_ERROR);

    m->allocated_cols = cols;
    m->allocated_rows = rows;

    return NEW_SUCCESS();
}

error_t dm_init(doubles_matrix_t *m, size_t rows, size_t cols)
{
    error_t err;
    if (is_failure(err = dm_allocate(m, rows, cols)))
        return err;

    m->rows = rows;
    m->cols = cols;

    return NEW_SUCCESS();
}

void dm_free(doubles_matrix_t *m)
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

bool initialized(doubles_matrix_t *m)
{
    return m && m->ptr && m->cols && m->rows && m->allocated_rows && m->allocated_cols;
}

error_t dm_fill_from_file(doubles_matrix_t *m, FILE *f)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", DM_ARGS_ERROR);

    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
            if (fscanf(f, "%lf", m->ptr[i] + j) != 1)
                return NEW_ERROR("error with reading", DM_READ_ERROR);

    return NEW_SUCCESS();
}

error_t dm_fill_by_matrix(doubles_matrix_t *m, doubles_matrix_t *src)
{
    if (!initialized(m) || !initialized(src))
        return NEW_ERROR("invalid matrix", DM_ARGS_ERROR);

    if (src->rows < m->rows || src->cols < m->cols)
        return NEW_ERROR("invalid size", DM_ARITHMETIC_ERROR);

    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
            m->ptr[i][j] = src->ptr[i][j];

    return NEW_SUCCESS();
}

error_t dm_fill_multiply(doubles_matrix_t *m, doubles_matrix_t *a, doubles_matrix_t *b)
{
    if (!initialized(m) || !initialized(a) || !initialized(b))
        return NEW_ERROR("invalid matrix", DM_ARGS_ERROR);

    if (a->cols != b->rows)
        return NEW_ERROR("invalid operands", DM_ARITHMETIC_ERROR);

    if (m->rows != a->rows || m->cols != b->cols)
        return NEW_ERROR("invalid size", DM_ARITHMETIC_ERROR);

    doubles_matrix_t buf;
    error_t err;
    if (is_failure(err = dm_init(&buf, m->rows, m->cols)))
        return err;

    for (size_t i = 0; i < buf.rows; i++)
        for (size_t j = 0; j < buf.cols; j++)
            for (size_t k = 0; k < a->cols; k++)
                buf.ptr[i][j] += a->ptr[i][k] * b->ptr[k][j];

    err = dm_fill_by_matrix(m, &buf);
    dm_free(&buf);
    return err;
}

error_t dm_fill_addition(doubles_matrix_t *m, doubles_matrix_t *a, doubles_matrix_t *b)
{
    if (!initialized(m) || !initialized(a) || !initialized(b))
        return NEW_ERROR("invalid matrix", DM_ARGS_ERROR);

    if (m->rows != a->rows || a->rows != b->rows)
        return NEW_ERROR("invalid size", DM_ARITHMETIC_ERROR);

    if (m->cols != a->cols || a->cols != b->cols)
        return NEW_ERROR("invalid size", DM_ARITHMETIC_ERROR);

    doubles_matrix_t buf;
    error_t err;
    if (is_failure(err = dm_init(&buf, m->rows, m->cols)))
        return err;

    for (size_t i = 0; i < buf.rows; i++)
        for (size_t j = 0; j < buf.cols; j++)
            buf.ptr[i][j] = a->ptr[i][j] + b->ptr[i][j];

    err = dm_fill_by_matrix(m, &buf);
    dm_free(&buf);
    return err;
}

error_t dm_print(doubles_matrix_t *m, FILE *f)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", DM_ARGS_ERROR);

    for (size_t i = 0; i < m->rows; i++)
    {
        for (size_t j = 0; j < m->cols; j++)
            if (fprintf(f, "%.6lf ", m->ptr[i][j]) < 0)
                return NEW_ERROR("error with printing", DM_PRINT_ERROR);
        fprintf(f, "\n");
    }

    return NEW_SUCCESS();
}

error_t create_minor(doubles_matrix_t *m, size_t row, size_t col, doubles_matrix_t *res)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", DM_ARGS_ERROR);

    if (m->cols <= 1 || m->rows <= 1)
        return NEW_ERROR("invalid size", DM_ARITHMETIC_ERROR);

    if (row >= m->rows || col >= m->cols)
        return NEW_ERROR("invalid index", DM_ARITHMETIC_ERROR);

    error_t err;
    if (is_failure(err = dm_init(res, m->rows - 1, m->cols - 1)))
        return err;

    size_t i, j;
    size_t im, jm;
    for (i = 0, im = 0; im < res->rows; i++, im++)
    {
        if (i == row)
            i++;
        for (j = 0, jm = 0; jm < res->cols; j++, jm++)
        {
            if (j == col)
                j++;
            res->ptr[im][jm] = m->ptr[i][j];
        }
    }

    return NEW_SUCCESS();
}

error_t dm_det(doubles_matrix_t *m, double *det)
{
    if (!initialized(m))
        return NEW_ERROR("invalid matrix", DM_ARGS_ERROR);

    if (m->cols != m->rows)
        return NEW_ERROR("invalid size", DM_ARITHMETIC_ERROR);

    if (m->rows == 1)
    {
        *det = m->ptr[0][0];
        return NEW_SUCCESS();
    }

    *det = 0;
    error_t err;
    int sign = 1;
    for (size_t j = 0; j < m->cols; j++)
    {
        doubles_matrix_t minor;
        double sub_det = 0;
        if (is_failure(err = create_minor(m, 0, j, &minor)))
            return err;
        err = dm_det(&minor, &sub_det);
        *det += sign * sub_det * m->ptr[0][j];
        sign *= -1;
        dm_free(&minor);
        if (is_failure(err))
            return err;
    }

    return NEW_SUCCESS();
}
