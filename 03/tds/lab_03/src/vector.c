#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <error.h>
#include <math.h>
#include <sys/time.h>

#include "vector.h"
#include "error.h"

bool v_allocated(v_vector_t *v)
{
    return v && v->ptr && v->allocated;
}

bool v_initialized(v_vector_t *v)
{
    return v_allocated(v) && v->len;
}

v_vector_t v_v()
{
    v_vector_t v = { 0 };
    return v;
}

error_t v_allocate(v_vector_t *v, size_t len)
{
    if (!v)
        return NEW_ERROR("нулевой указатель", V_ARGS_ERROR);

    if (!len)
        return NEW_ERROR("некорректные размеры", V_ARGS_ERROR);

    if (v_allocated(v))
        return NEW_ERROR("вектор уже выделен", V_ARGS_ERROR);

    v->ptr = calloc(len, sizeof(double *));
    if (!v->ptr)
        return NEW_ERROR("ошибка выделения памяти", V_ALLOC_ERROR);

    v->allocated = len;

    return NEW_SUCCESS();
}

error_t v_init(v_vector_t *v, size_t len)
{
    if (v_initialized(v))
        return NEW_ERROR("вектор уже инициализирован", V_ARGS_ERROR);

    error_t err;
    if (is_failure(err = v_allocate(v, len)))
        return err;

    v->len = len;

    return NEW_SUCCESS();
}

void swap_size_t(size_t *a, size_t *b)
{
    size_t t = *a;
    *a = *b;
    *b = t;
}

error_t v_generate(v_vector_t *v)
{
    if (!v_initialized(v))
        return NEW_ERROR("вектор уже инициализирован", V_ARGS_ERROR);

    size_t *positions = calloc(v->len, sizeof(size_t));
    if (!positions)
        return NEW_ERROR("ошибка выделения памяти под массив", V_ALLOC_ERROR);

    for (size_t i = 0; i < v->len; i++)
        positions[i] = i;

    srand(time(NULL));
    size_t num = rand() % (v->len + 1 - 3 * v->len / 4) + 3 * v->len / 4;
    size_t all = v->len;
    for (size_t k = 0; k < num; k++)
    {
        size_t index = rand() % all;
        v->ptr[positions[index]] = rand() % V_RANDOM_ELEMENT_BORDER;
        swap_size_t(positions + index, positions + all - 1);
        all--;
    }
    free(positions);

    return NEW_SUCCESS();
}

void v_free(v_vector_t *v)
{
    if (v && v->ptr)
    {
        free(v->ptr);
        v->ptr = NULL;
        v->len = 0;
        v->allocated = 0;
    }
}

error_t v_from_file(v_vector_t *v, FILE *f)
{
    if (!f)
        return NEW_ERROR("передан нулевой указатель", V_ARGS_ERROR);

    if (v_allocated(v))
        return NEW_ERROR("память под вектор уже выделена", V_ARGS_ERROR);

    error_t err;
    size_t len;
    if (is_failure(err = v_read_len(&len, f)))
        return err;

    if (is_failure(err = v_init(v, len)))
        return err;

    if (is_failure(err = v_read(v, f)))
    {
        v_free(v);
        return err;
    }

    return NEW_SUCCESS();
}

error_t v_read_len(size_t *len, FILE *f)
{
    if (!len || !f)
        return NEW_ERROR("передан нулевой указатель", V_ARGS_ERROR);

    if (fscanf(f, "%zu", len) != 1)
        return NEW_ERROR("ошибка чтения длины вектора", V_READ_ERROR);

    return NEW_SUCCESS();
}

error_t v_read(v_vector_t *v, FILE *f)
{
    if (!f)
        return NEW_ERROR("передан нулевой указатель", V_ARGS_ERROR);

    if (!v_initialized(v))
        return NEW_ERROR("передан некорректный вектор", V_ARGS_ERROR);

    for (size_t i = 0; i < v->len; i++)
        if (fscanf(f, "%lf", v->ptr + i) != 1)
            return NEW_ERROR("ошибка чтения вектора", V_READ_ERROR);

    return NEW_SUCCESS();
}

error_t v_write(v_vector_t *v, FILE *f)
{
    if (!f)
        return NEW_ERROR("передан нулевой указатель", V_ARGS_ERROR);

    if (!v_initialized(v))
        return NEW_ERROR("передан некорректный вектор", V_ARGS_ERROR);

    for (size_t i = 0; i < v->len; i++)
    {
        if (fprintf(f, "%.1lf\n", v->ptr[i]) < 0)
            return NEW_ERROR("ошибка вывода", V_PRINT_ERROR);
    }

    return NEW_SUCCESS();
}
