#include <stdio.h>
#include <sys/time.h>

#include "benchmark.h"
#include "error.h"
#include "stack_list.h"
#include "stack_arr.h"
#include "mystring.h"

#define CHECK_TIME(p,t) struct timeval tv_start, tv_stop; \
gettimeofday(&tv_start, NULL); \
p; \
gettimeofday(&tv_stop, NULL); \
t += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);

error_t bench_stack_arr_push(size_t n, long long *timer)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;
    mystring_t str = "test";
    error_t err = NEW_SUCCESS();
    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        stack_array_t sa = sa_create();
        for (size_t q = 0; q < n; q++)
        {
            CHECK_TIME(err = sa_push(&sa, str), *timer);
            if (is_failure(err))
                return err;
        }
    }

    *timer /= BENCH_NUM_OF_RUNS;
    return NEW_SUCCESS();
}

error_t bench_stack_arr_pop(size_t n, long long *timer)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;
    mystring_t str = "test";
    error_t err;
    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        stack_array_t sa = sa_create();
        for (size_t q = 0; q < n; q++)
        {
            err = sa_push(&sa, str);
            if (is_failure(err))
                return err;
        }

        mystring_t buf;
        for (size_t q = 0; q < n; q++)
        {
            CHECK_TIME(err = sa_pop(&sa, buf), *timer);
            if (is_failure(err))
                return err;
        }
    }

    *timer /= BENCH_NUM_OF_RUNS;
    return NEW_SUCCESS();
}

error_t bench_stack_list_push(size_t n, long long *timer)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;
    mystring_t str = "test";
    error_t err;
    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        stack_list_t *sl = NULL;
        for (size_t q = 0; q < n; q++)
        {
            CHECK_TIME(err = sl_push(&sl, str), *timer);
            if (is_failure(err))
            {
                sl_free(&sl);
                return err;
            }
        }
        sl_free(&sl);
    }


    *timer /= BENCH_NUM_OF_RUNS;
    return NEW_SUCCESS();
}

error_t bench_stack_list_pop(size_t n, long long *timer)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;
    mystring_t str = "test";
    error_t err;
    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        stack_list_t *sl = NULL;
        for (size_t q = 0; q < n; q++)
        {
            err = sl_push(&sl, str);
            if (is_failure(err))
            {
                sl_free(&sl);
                return err;
            }
        }

        mystring_t buf;
        for (size_t q = 0; q < n; q++)
        {
            CHECK_TIME(err = sl_pop(&sl, buf), *timer);
            if (is_failure(err))
            {
                sl_free(&sl);
                return err;
            }
        }
    }

    *timer /= BENCH_NUM_OF_RUNS;
    return NEW_SUCCESS();
}
