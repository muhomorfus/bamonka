#include <stdio.h>
#include <sys/time.h>

#include "benchmark.h"
#include "error.h"
#include "queue_list.h"
#include "queue_arr.h"

#define CHECK_TIME(p,t) struct timeval tv_start, tv_stop; \
gettimeofday(&tv_start, NULL); \
p; \
gettimeofday(&tv_stop, NULL); \
t += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);

error_t bench_queue_arr_push(size_t n, long long *timer)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;
    error_t err = NEW_SUCCESS();
    task_t task = { 0 };
    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        queue_array_t qa = qa_create();
        for (size_t q = 0; q < n; q++)
        {
            CHECK_TIME(err = qa_push(&qa, task), *timer);
            if (is_failure(err))
                return err;
        }
    }

    *timer /= BENCH_NUM_OF_RUNS;
    return NEW_SUCCESS();
}

error_t bench_queue_arr_pop(size_t n, long long *timer)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;
    task_t task = { 0 };
    error_t err;
    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        queue_array_t qa = qa_create();
        for (size_t q = 0; q < n; q++)
        {
            err = qa_push(&qa, task);
            if (is_failure(err))
                return err;
        }

        for (size_t q = 0; q < n; q++)
        {
            CHECK_TIME(err = qa_pop(&qa, &task), *timer);
            if (is_failure(err))
                return err;
        }
    }

    *timer /= BENCH_NUM_OF_RUNS;
    return NEW_SUCCESS();
}

error_t bench_queue_list_push(size_t n, long long *timer)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;
    task_t task = { 0 };
    error_t err;
    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        queue_list_t ql = ql_create();
        for (size_t q = 0; q < n; q++)
        {
            CHECK_TIME(err = ql_push(&ql, task), *timer);
            if (is_failure(err))
            {
                ql_free(&ql);
                return err;
            }
        }
        ql_free(&ql);
    }


    *timer /= BENCH_NUM_OF_RUNS;
    return NEW_SUCCESS();
}

error_t bench_queue_list_pop(size_t n, long long *timer)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;
    task_t task = { 0 };
    error_t err;
    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        queue_list_t ql = ql_create();
        for (size_t q = 0; q < n; q++)
        {
            err = ql_push(&ql, task);
            if (is_failure(err))
            {
                ql_free(&ql);
                return err;
            }
        }

        for (size_t q = 0; q < n; q++)
        {
            CHECK_TIME(err = ql_pop(&ql, &task), *timer);
            if (is_failure(err))
            {
                ql_free(&ql);
                return err;
            }
        }
    }

    *timer /= BENCH_NUM_OF_RUNS;
    return NEW_SUCCESS();
}
