#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "csc_matrix.h"
#include "usual_matrix.h"
#include "vector.h"
#include "error.h"

#define EMPTY ((size_t) -1)

#define EPS 1e-6

bool csc_allocated(csc_matrix_t *m)
{
    return m && m->rows && m->values && m->allocated_n && m->col_list && m->allocated_cols;
}

bool csc_initialized(csc_matrix_t *m)
{
    return csc_allocated(m) && m->num_of_cols;
}

csc_matrix_t csc_csc()
{
    csc_matrix_t m = { 0 };
    return m;
}

void csc_free(csc_matrix_t *m)
{
    free(m->values);
    m->values = NULL;
    free(m->rows);
    m->rows = NULL;
    stl_free(&m->col_list);
    m->allocated_n = 0;
    m->n = 0;
    m->allocated_cols = 0;
    m->num_of_cols = 0;
}

error_t csc_allocate(csc_matrix_t *m, size_t n, size_t cols)
{
    if (csc_allocated(m))
        return NEW_ERROR("разреженная матрица уже выделена", csc_ARGS_ERROR);

    if (!cols)
        return NEW_ERROR("некорректные размеры", csc_ARGS_ERROR);

    if (!n)
        n++;
    cols++;

    m->values = calloc(1, n * sizeof(double));
    if (!m->values)
        return NEW_ERROR("ошибка выделения памяти", csc_ALLOC_ERROR);

    m->rows = calloc(1, n * sizeof(size_t));
    if (!m->rows)
    {
        csc_free(m);
        return NEW_ERROR("ошибка выделения памяти", csc_ALLOC_ERROR);
    }

    m->col_list = NULL;
    stl_list_t *list = NULL;
    error_t err;
    if (is_failure(err = stl_append_to_head(&m->col_list, 0, &list)))
    {
        csc_free(m);
        return err;
    }

    for (size_t k = 1; k < cols; k++)
        if (is_failure(err = stl_append_to_element(&list, 0, &list)))
        {
            csc_free(m);
            return err;
        }

    m->allocated_n = n;
    m->allocated_cols = cols;

    return NEW_SUCCESS();
}

error_t csc_init(csc_matrix_t *m, size_t n, size_t rows, size_t cols)
{
    error_t err;
    if (is_failure(err = csc_allocate(m, n, cols)))
        return err;

    m->n = n;
    m->num_of_cols = cols;
    m->num_of_rows = rows;

    return NEW_SUCCESS();
}

error_t csc_from_um(csc_matrix_t *m, um_matrix_t *src)
{
    if (!um_initialized(src))
        return NEW_ERROR("матрица не инициализирована", csc_ARGS_ERROR);

    if (csc_allocated(m))
        return NEW_ERROR("разреженная матрица не должна быть выделена", csc_ARGS_ERROR);

    error_t err;
    size_t n;
    if (is_failure(err = um_count_non_zeros(src, &n)))
        return err;

    if (is_failure(err = csc_init(m, n, src->rows, src->cols)))
        return err;

    stl_list_t *current =  m->col_list;
    size_t pos = 0;
    for (size_t j = 0; j < src->cols; j++)
    {
        size_t first = EMPTY;
        for (size_t i = 0; i < src->rows; i++)
        {
            if (fabs(src->ptr[i][j]) >= EPS)
            {
                m->values[pos] = src->ptr[i][j];
                m->rows[pos] = i;
                if (first == EMPTY)
                    first = pos;
                pos++;
            }
        }
        current->value = first;
        current = current->next;
    }
    current->value = pos;

    stl_list_t *first_empty = NULL;
    for (stl_list_t *element = m->col_list; element; element = element->next)
    {
        if (!first_empty && element->value == EMPTY)
            first_empty = element;
        if (first_empty && element->value != EMPTY)
        {
            while (first_empty != element)
            {
                first_empty->value = element->value;
                first_empty = first_empty->next;
            }
            first_empty = NULL;
        }
    }

    return NEW_SUCCESS();
}

error_t csc_from_v(csc_matrix_t *m, v_vector_t *src)
{
    if (!v_initialized(src))
        return NEW_ERROR("вектор не инициализирован", csc_ARGS_ERROR);

    if (csc_allocated(m))
        return NEW_ERROR("разреженная матрица не должна быть выделена", csc_ARGS_ERROR);

    error_t err;
    um_matrix_t buf = um_um();

    if (is_failure(err = um_from_v(&buf, src)))
        return err;

    err = csc_from_um(m, &buf);
    um_free(&buf);
    return err;
}

error_t csc_write(csc_matrix_t *m, FILE *f)
{
    if (!f)
        return NEW_ERROR("передан нулевой указатель", csc_ARGS_ERROR);

    if (!csc_initialized(m))
        return NEW_ERROR("разреженная матрица не инициализирована", csc_ARGS_ERROR);

    if (fprintf(f, "A: ") < 0)
        return NEW_ERROR("ошибка вывода", csc_WRITE_ERROR);

    for (size_t i = 0; i < m->n; i++)
        if (fprintf(f, "%.1lf ", m->values[i]) < 0)
            return NEW_ERROR("ошибка вывода", csc_WRITE_ERROR);

    if (fprintf(f, "\nIA: ") < 0)
        return NEW_ERROR("ошибка вывода", csc_WRITE_ERROR);

    for (size_t i = 0; i < m->n; i++)
        if (fprintf(f, "%zu ", m->rows[i]) < 0)
            return NEW_ERROR("ошибка вывода", csc_WRITE_ERROR);

    if (fprintf(f, "\nJA: ") < 0)
        return NEW_ERROR("ошибка вывода", csc_WRITE_ERROR);

    for (stl_list_t *element = m->col_list; element; element = element->next)
        if (fprintf(f, "%zu ", element->value) < 0)
            return NEW_ERROR("ошибка вывода", csc_WRITE_ERROR);

    printf("\n");
    return NEW_SUCCESS();
}

error_t csc_multiply_to_v(v_vector_t *v, csc_matrix_t *a, v_vector_t *b)
{
    if (v_allocated(v))
        return NEW_ERROR("вектор не должен быть выделен", UM_ARGS_ERROR);

    if (!csc_initialized(a) || !v_initialized(b))
        return NEW_ERROR("переданы неверные операнды", UM_ARGS_ERROR);

    if (a->num_of_cols != b->len)
        return NEW_ERROR("неверные размеры", UM_ARITHMETIC_ERROR);

    error_t err;
    if (is_failure(err = v_init(v, a->num_of_rows)))
        return err;

    size_t i = 0;
    for (stl_list_t *element = a->col_list; element->next; element = element->next, i++)
    {
        size_t begin = element->value;
        size_t end = element->next->value;
        for (size_t k = begin; k < end; k++)
            v->ptr[a->rows[k]] += b->ptr[i] * a->values[k];
    }

    return NEW_SUCCESS();
}

error_t csc_multiply_with_v(csc_matrix_t *res, csc_matrix_t *a, v_vector_t *b)
{
    if (csc_allocated(res))
        return NEW_ERROR("матрица не должна быть выделена", UM_ARGS_ERROR);

    if (!csc_initialized(a) || !v_initialized(b))
        return NEW_ERROR("переданы неверные операнды", UM_ARGS_ERROR);

    error_t err;
    v_vector_t buf = v_v();
    if (is_failure(err = csc_multiply_to_v(&buf, a, b)))
        return err;

    err = csc_from_v(res, &buf);
    v_free(&buf);
    return err;
}


