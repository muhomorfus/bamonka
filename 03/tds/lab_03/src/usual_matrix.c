#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <error.h>
#include <math.h>
#include <sys/time.h>

#include "usual_matrix.h"
#include "vector.h"
#include "error.h"

#define PLACE 7

#define EPS 1e-6

#define MAX_ELEM 100

typedef struct pair_t
{
    size_t i;
    size_t j;
} pair_t;

bool um_allocated(um_matrix_t *m)
{
    return m && m->ptr && m->allocated_rows && m->allocated_cols;
}

bool um_initialized(um_matrix_t *m)
{
    return um_allocated(m) && m->cols && m->rows;
}

um_matrix_t um_um()
{
    um_matrix_t m = { 0 };
    return m;
}

error_t um_allocate(um_matrix_t *m, size_t rows, size_t cols)
{
    if (!m)
        return NEW_ERROR("нулевой указатель", UM_ARGS_ERROR);

    if (!rows || !cols)
        return NEW_ERROR("некорректные размеры", UM_ARGS_ERROR);

    if (um_allocated(m))
        return NEW_ERROR("матрица уже выделена", UM_ARGS_ERROR);

    m->ptr = calloc(rows, sizeof(double *));
    if (!m->ptr)
        return NEW_ERROR("ошибка выделения памяти", UM_ALLOC_ERROR);

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
        return NEW_ERROR("ошибка выделения памяти на строки", UM_ALLOC_ERROR);

    m->allocated_cols = cols;
    m->allocated_rows = rows;

    return NEW_SUCCESS();
}

error_t um_init(um_matrix_t *m, size_t rows, size_t cols)
{
    if (um_initialized(m))
        return NEW_ERROR("матрица уже инициализирована", UM_ARGS_ERROR);

    error_t err;
    if (is_failure(err = um_allocate(m, rows, cols)))
        return err;

    m->rows = rows;
    m->cols = cols;

    return NEW_SUCCESS();
}

void um_free(um_matrix_t *m)
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

void swap_pairs(pair_t *a, pair_t *b)
{
    pair_t tmp = *a;
    *a = *b;
    *b = tmp;
}

error_t um_generate(um_matrix_t *m, int percent)
{
    if (!m)
        return NEW_ERROR("передан нулевой указатель", UM_ARGS_ERROR);

    if (percent < 0 || percent > 100)
        return NEW_ERROR("неверный процент заполненности", UM_ARGS_ERROR);

    if (!um_initialized(m))
        return NEW_ERROR("память под матрицу уже выделена", UM_ARGS_ERROR);

    size_t all = m->rows * m->cols;
    size_t non_zeros = all * percent / 100;
    pair_t *positions = calloc(all, sizeof(pair_t));
    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
        {
            positions[i * m->cols + j].i = i;
            positions[i * m->cols + j].j = j;
        }

    srand(time(NULL));
    for (size_t k = 0; k < non_zeros; k++)
    {
        size_t index = rand() % all;
        m->ptr[positions[index].i][positions[index].j] = rand() % MAX_ELEM;
        swap_pairs(positions + index, positions + all - 1);
        all--;
    }

    free(positions);
    return NEW_SUCCESS();
}

error_t um_from_file(um_matrix_t *m, FILE *f)
{
    if (!f)
        return NEW_ERROR("передан нулевой указатель", UM_ARGS_ERROR);

    if (um_allocated(m))
        return NEW_ERROR("память под матрицу уже выделена", UM_ARGS_ERROR);

    error_t err;
    size_t rows, cols;
    if (is_failure(err = um_read_size(&rows, &cols, f)))
        return err;

    if (is_failure(err = um_init(m, rows, cols)))
        return err;

    if (is_failure(err = um_read(m, f)))
    {
        um_free(m);
        return err;
    }

    return NEW_SUCCESS();
}

error_t um_from_file_coordinates(um_matrix_t *m, FILE *f)
{
    if (!f)
        return NEW_ERROR("передан нулевой указатель", UM_ARGS_ERROR);

    if (um_allocated(m))
        return NEW_ERROR("память под матрицу уже выделена", UM_ARGS_ERROR);

    error_t err;
    size_t rows, cols;
    if (is_failure(err = um_read_size(&rows, &cols, f)))
        return err;

    size_t not_zero;
    if (fscanf(f, "%zu", &not_zero) != 1)
        return NEW_ERROR("ошибка чтения", UM_READ_ERROR);

    if (is_failure(err = um_init(m, rows, cols)))
        return err;

    if (is_failure(err = um_read_coordinates(m, f, not_zero)))
    {
        um_free(m);
        return err;
    }

    return NEW_SUCCESS();
}

error_t um_read_size(size_t *rows, size_t *cols, FILE *f)
{
    if (!rows || !cols || !f)
        return NEW_ERROR("передан нулевой указатель", UM_ARGS_ERROR);

    if (fscanf(f, "%zux%zu", rows, cols) != 2)
        return NEW_ERROR("ошибка чтения размеров матрицы", UM_READ_ERROR);

    return NEW_SUCCESS();
}

error_t um_read_coordinates(um_matrix_t *m, FILE *f, size_t not_zero)
{
    if (!f)
        return NEW_ERROR("передан нулевой указатель", UM_ARGS_ERROR);

    if (!um_initialized(m))
        return NEW_ERROR("передана некорректная матрица", UM_ARGS_ERROR);

    if (not_zero > m->rows * m->cols)
        return NEW_ERROR("неверное количество ненулевых элементов", UM_READ_ERROR);

    for (size_t k = 0; k < not_zero; k++)
    {
        size_t i, j;
        double element;
        if (fscanf(f, "%zu%zu%lf", &i, &j, &element) != 3)
            return NEW_ERROR("ошибка чтения", UM_READ_ERROR);
        if (i >= m->rows || j >= m->cols)
            return NEW_ERROR("индексы выходят за пределы матрицы", UM_OUT_OF_RANGE_ERROR);
        if (fabs(m->ptr[i][j]) > EPS)
            return NEW_ERROR("элемент матрицы уже заполнен", UM_REPEAT_ERROR);
        if (fabs(element) < EPS)
            return NEW_ERROR("вводимый элемент не должен быть нулевым", UM_ARITHMETIC_ERROR);

        m->ptr[i][j] = element;
    }

    return NEW_SUCCESS();
}

error_t um_read(um_matrix_t *m, FILE *f)
{
    if (!f)
        return NEW_ERROR("передан нулевой указатель", UM_ARGS_ERROR);

    if (!um_initialized(m))
        return NEW_ERROR("передана некорректная матрица", UM_ARGS_ERROR);

    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
            if (fscanf(f, "%lf", m->ptr[i] + j) != 1)
                return NEW_ERROR("ошибка чтения матрицы", UM_READ_ERROR);

    return NEW_SUCCESS();
}

error_t um_write(um_matrix_t *m, FILE *f)
{
    if (!f)
        return NEW_ERROR("передан нулевой указатель", UM_ARGS_ERROR);

    if (!um_initialized(m))
        return NEW_ERROR("передана некорректная матрица", UM_ARGS_ERROR);

    for (size_t i = 0; i < m->rows; i++)
    {
        int printed;
        for (size_t j = 0; j < m->cols; j++)
        {
            if ((printed = fprintf(f, "%.1lf", m->ptr[i][j])) < 0)
                return NEW_ERROR("ошибка вывода", UM_PRINT_ERROR);
            for (int k = printed; k < PLACE; k++)
                if (fprintf(f, " ") < 0)
                    return NEW_ERROR("ошибка вывода", UM_PRINT_ERROR);
        }
        fprintf(f, "\n");
    }

    return NEW_SUCCESS();
}

error_t um_multiply(um_matrix_t *m, um_matrix_t *a, um_matrix_t *b)
{
    if (um_allocated(m) || !um_initialized(a) || !um_initialized(b))
        return NEW_ERROR("передана неверная матрица", UM_ARGS_ERROR);

    if (a->cols != b->rows)
        return NEW_ERROR("неверные размеры матриц", UM_ARITHMETIC_ERROR);

    error_t err;
    if (is_failure(err = um_init(m, a->rows, b->cols)))
        return err;

    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
            for (size_t k = 0; k < a->cols; k++)
                m->ptr[i][j] += a->ptr[i][k] * b->ptr[k][j];

    return NEW_SUCCESS();
}

error_t um_multiply_to_v(v_vector_t *v, um_matrix_t *a, v_vector_t *b)
{
    if (v_allocated(v))
        return NEW_ERROR("вектор не должен быть выделен", UM_ARGS_ERROR);

    if (!um_initialized(a) || !v_initialized(b))
        return NEW_ERROR("переданы неверные операнды", UM_ARGS_ERROR);

    if (a->cols != b->len)
        return NEW_ERROR("неверные размеры", UM_ARITHMETIC_ERROR);

    error_t err;
    if (is_failure(err = v_init(v, a->rows)))
        return err;

    for (size_t i = 0; i < v->len; i++)
        for (size_t k = 0; k < b->len; k++)
            v->ptr[i] += a->ptr[i][k] * b->ptr[k];

    return NEW_SUCCESS();
}

error_t um_count_non_zeros(um_matrix_t *m, size_t *non_zeros)
{
    if (!non_zeros)
        return NEW_ERROR("передан нулевой указатель", UM_ARGS_ERROR);

    if (!um_initialized(m))
        return NEW_ERROR("передана некорректная матрица", UM_ARGS_ERROR);

    *non_zeros = 0;
    for (size_t i = 0; i < m->rows; i++)
        for (size_t j = 0; j < m->cols; j++)
            if (fabs(m->ptr[i][j]) > EPS)
                (*non_zeros)++;

    return NEW_SUCCESS();
}

error_t um_from_v(um_matrix_t *m, v_vector_t *v)
{
    if (um_allocated(m))
        return NEW_ERROR("матрица не должна быть выделена", UM_ARGS_ERROR);

    if (!v_initialized(v))
        return NEW_ERROR("передан неинициализированный вектор", UM_ARGS_ERROR);

    error_t err;
    if (is_failure(err = um_init(m, v->len, 1)))
        return err;

    for (size_t i = 0; i < v->len; i++)
        m->ptr[i][0] = v->ptr[i];

    return NEW_SUCCESS();
}

error_t um_multiply_with_v(um_matrix_t *res, um_matrix_t *a, v_vector_t *b)
{
    if (um_allocated(res))
        return NEW_ERROR("матрица не должна быть выделена", UM_ARGS_ERROR);

    if (!um_initialized(a) || !v_initialized(b))
        return NEW_ERROR("переданы неверные операнды", UM_ARGS_ERROR);

    error_t err;
    v_vector_t buf = v_v();
    if (is_failure(err = um_multiply_to_v(&buf, a, b)))
        return err;

    err = um_from_v(res, &buf);
    v_free(&buf);

    return err;
}

void swap(size_t *a, size_t *b)
{
    size_t tmp = *a;
    *a = *b;
    *b = tmp;
}

error_t um_random(um_matrix_t *m, size_t rows, size_t cols, int percent)
{
    if (um_allocated(m))
        return NEW_ERROR("матрица не должна быть выделена", UM_ARGS_ERROR);

    if (percent < 0)
        return NEW_ERROR("неверный процент заполненности", UM_ARGS_ERROR);

    if (!rows || !cols)
        return NEW_ERROR("неверные размеры", UM_ARGS_ERROR);

    error_t err;
    if (is_failure(err = um_init(m, rows, cols)))
        return NEW_ERROR("ошибка выделения памяти", UM_ALLOC_ERROR);

    size_t *positions = calloc(rows * cols, sizeof(size_t));
    if (!positions)
    {
        um_free(m);
        return NEW_ERROR("ошибка выделения памяти", UM_ALLOC_ERROR);
    }

    for (size_t i = 0; i < rows * cols; i++)
        positions[i] = i;

    size_t last = rows * cols - 1;
    size_t num = rows * cols * percent / 100;
    for (size_t k = 0; k < num; k++)
    {
        size_t index = rand() % (last + 1);
        m->ptr[positions[index] / cols][positions[index] % cols] = rand() % UM_RANDOM_ELEMENT_BORDER;
        swap(positions + index, positions + last);
        last--;
    }

    free(positions);
    return NEW_SUCCESS();
}

