#include <stdio.h>
#include <sys/time.h>

#include "benchmark.h"
#include "error.h"
#include "usual_matrix.h"
#include "csc_matrix.h"
#include "vector.h"

#define CHECK_TIME(p,t) struct timeval tv_start, tv_stop; \
gettimeofday(&tv_start, NULL); \
p; \
gettimeofday(&tv_stop, NULL); \
t = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);

//#define CHECK_TIME(p,t) clock_t start = clock(); \
//p;                                               \
//t = clock() - start;

error_t bench_um_memory_usage(um_matrix_t *m, size_t *size)
{
    if (!um_initialized(m))
        return NEW_ERROR("неверная матрица", BENCH_ARGS_ERROR);

    if (!size)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *size = 0;
    *size += sizeof(m->allocated_rows) + sizeof(m->allocated_cols);
    *size += sizeof(m->rows) + sizeof(m->cols);
    *size += sizeof(m->ptr);
    *size += m->allocated_cols * m->allocated_rows * sizeof(double);

    return NEW_SUCCESS();
}

error_t bench_csc_memory_usage(csc_matrix_t *cm, size_t *size)
{
    if (!csc_initialized(cm))
        return NEW_ERROR("неверная матрица", BENCH_ARGS_ERROR);

    if (!size)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *size = 0;
    *size += sizeof(cm->allocated_cols) + sizeof(cm->allocated_n);
    *size += sizeof(cm->num_of_rows) + sizeof(cm->num_of_cols);
    *size += sizeof(cm->n);
    *size += sizeof(cm->rows) + sizeof(cm->values);
    *size += cm->allocated_n * sizeof(size_t);
    *size += cm->allocated_n * sizeof(double);
    *size += cm->allocated_cols * sizeof(*cm->col_list);

    return NEW_SUCCESS();
}

error_t um_mul_time(um_matrix_t *m, v_vector_t *v, long long *time)
{
    if (!time)
        return NEW_ERROR("неверные аргументы", BENCH_ARGS_ERROR);

    um_matrix_t result = um_um();
    error_t err;
    CHECK_TIME(err = um_multiply_with_v(&result, m, v), *time);
    um_free(&result);
    return err;
}

error_t csc_mul_time(csc_matrix_t *cm, v_vector_t *v, long long *time)
{
    if (!time)
        return NEW_ERROR("неверные аргументы", BENCH_ARGS_ERROR);

    csc_matrix_t result = csc_csc();
    error_t err;
    CHECK_TIME(err = csc_multiply_with_v(&result, cm, v), *time);
    csc_free(&result);
    return err;
}

error_t bench_um_mul(um_matrix_t *m, v_vector_t *v, long long *sum_time)
{
    *sum_time = 0;
    long long t;
    error_t err;
    for (size_t i = 0; i < BENCH_NUM_OF_RUNS; i++)
    {
        if (is_failure(err = um_mul_time(m, v, &t)))
            return err;
        *sum_time += t;
    }

    return NEW_SUCCESS();
}

error_t bench_csc_mul(csc_matrix_t *cm, v_vector_t *v, long long *sum_time)
{
    *sum_time = 0;
    long long t;
    error_t err;
    for (size_t i = 0; i < BENCH_NUM_OF_RUNS; i++)
    {
        if (is_failure(err = csc_mul_time(cm, v, &t)))
            return err;
        *sum_time += t;
    }

    return NEW_SUCCESS();
}
