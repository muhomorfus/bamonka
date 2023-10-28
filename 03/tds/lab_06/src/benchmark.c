#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "benchmark.h"
#include "error.h"
#include "mystring.h"
#include "bst.h"
#include "avl.h"
#include "ht.h"
#include "file.h"

static error_t new_tmp_filename(mystring_t filename)
{
    if (!filename)
        return NEW_ERROR("передан нулевой указатель", BST_ARGS_ERROR);

    if (sprintf(filename, "/tmp/%ld_%d.data.txt", time(NULL), rand()) < 0)
        return NEW_ERROR("ошибка имени файла", BST_PRINT_ERROR);

    return NEW_SUCCESS();
}

static error_t fill_file(mystring_t filename, size_t n)
{
    mystring_t cmd;
    sprintf(cmd, "echo %zu | python3 generator.py %s", n, filename);
    system(cmd);
    return NEW_SUCCESS();
}

int string_cmp(const void *a, const void *b)
{
    const char *a_str = a;
    const char *b_str = b;
    return strcmp(a_str, b_str);
}

mystring_t *get_random_strings(size_t n)
{
    srand(bench_tick());
    
    mystring_t filename;
    if (is_failure(new_tmp_filename(filename)))
        return NULL;
    
    if (is_failure(fill_file(filename, n)))
        return NULL;

    mystring_t *sorted = calloc(n, sizeof(mystring_t));
    if (!sorted)
        return NULL;
    
    FILE *f = fopen(filename, "r");
    for (size_t i = 0; i < n; i++)
    {
        mystring_t buf;
        if (is_failure(ms_read_word(f, buf)))
        {
            fclose(f);
            return NULL;
        }
        
        strcpy(sorted[i], buf);
    }
    fclose(f);
    qsort(sorted, n, sizeof(mystring_t), string_cmp);
    
    return sorted;
}

error_t bench_bst_remove(size_t n, long long *timer, size_t *n_cmp)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;

    mystring_t *arr = get_random_strings(n);
    if (!arr)
        return NEW_ERROR("ошибка создания массива строк", BENCH_ALLOC_ERROR);

    error_t err = NEW_SUCCESS();

    bst_t *bst = NULL;
    for (size_t i = 0; i < n; i++)
        if (is_failure(err = bst_insert(&bst, arr[i])))
        {
            free(arr);
            return err;
        }
    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        for (size_t i = 0; i < n; i++)
        {
            long long int cur_timer = 0;
            size_t cur_cmp = 0;
            BENCH_CHECK_TIME(err = bst_remove(&bst, arr[i], &cur_cmp), cur_timer);
            *n_cmp += cur_cmp;
            *timer += cur_timer;
            if (is_failure(err))
            {
                free(arr);
                bst_free(&bst);
                return err;
            }

            if (is_failure(err = bst_insert(&bst, arr[i])))
            {
                free(arr);
                bst_free(&bst);
                return err;
            }
        }
    }

    *timer /= BENCH_NUM_OF_RUNS * n;
    *n_cmp /= BENCH_NUM_OF_RUNS * n;
    bst_free(&bst);
    return NEW_SUCCESS();
}

error_t bench_bst_search(size_t n, long long *timer, size_t *n_cmp)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;

    mystring_t *arr = get_random_strings(n);
    if (!arr)
        return NEW_ERROR("ошибка создания массива строк", BENCH_ALLOC_ERROR);

    error_t err = NEW_SUCCESS();

    bst_t *bst = NULL;
    for (size_t i = 0; i < n; i++)
        if (is_failure(err = bst_insert(&bst, arr[i])))
        {
            free(arr);
            return err;
        }

    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        for (size_t i = 0; i < n; i++)
        {
            long long int cur_timer = 0;
            bst_t *found = NULL;
            size_t cur_cmp = 0;
            BENCH_CHECK_TIME(err = bst_search(&bst, arr[i], &found, &cur_cmp), cur_timer);
            *n_cmp += cur_cmp;
            *timer += cur_timer;
            if (is_failure(err) && err.code != BST_NOT_FOUND_ERROR)
            {
                free(arr);
                bst_free(&bst);
                return err;
            }
        }
    }

    *timer /= BENCH_NUM_OF_RUNS * n;
    *n_cmp /= BENCH_NUM_OF_RUNS * n;
    bst_free(&bst);
    return NEW_SUCCESS();
}

error_t bench_avl_remove(size_t n, long long *timer, size_t *n_cmp)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;

    mystring_t *arr = get_random_strings(n);
    if (!arr)
        return NEW_ERROR("ошибка создания массива строк", BENCH_ALLOC_ERROR);

    error_t err = NEW_SUCCESS();
    avl_t *avl = NULL;
    for (size_t i = 0; i < n; i++)
        if (is_failure(err = avl_insert(&avl, arr[i])))
        {
            free(arr);
            return err;
        }

    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        for (size_t i = 0; i < n; i++)
        {
            long long int cur_timer = 0;
            size_t cur_cmp = 0;
            BENCH_CHECK_TIME(err = avl_remove(&avl, arr[n - 1], &cur_cmp), cur_timer);
            *n_cmp += cur_cmp;
            *timer += cur_timer;
            if (is_failure(err))
            {
                free(arr);
                avl_free(&avl);
                return err;
            }

            if (is_failure(err = avl_insert(&avl, arr[n - 1])))
            {
                free(arr);
                avl_free(&avl);
                return err;
            }
        }
    }

    *timer /= BENCH_NUM_OF_RUNS * n;
    *n_cmp /= BENCH_NUM_OF_RUNS * n;
    avl_free(&avl);
    return NEW_SUCCESS();
}

error_t bench_avl_search(size_t n, long long *timer, size_t *n_cmp)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;

    mystring_t *arr = get_random_strings(n);
    if (!arr)
        return NEW_ERROR("ошибка создания массива строк", BENCH_ALLOC_ERROR);

    error_t err = NEW_SUCCESS();

    avl_t *avl = NULL;
    for (size_t i = 0; i < n; i++)
        if (is_failure(err = avl_insert(&avl, arr[i])))
        {
            free(arr);
            return err;
        }

    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        for (size_t i = 0; i < n; i++)
        {
            long long int cur_timer = 0;
            size_t cur_cmp = 0;
            avl_t *found = NULL;
            BENCH_CHECK_TIME(err = avl_search(&avl, arr[n - 1], &found, &cur_cmp), cur_timer);
            *n_cmp += cur_cmp;
            *timer += cur_timer;
            if (is_failure(err) && err.code != AVL_NOT_FOUND_ERROR)
            {
                free(arr);
                avl_free(&avl);
                return err;
            }
        }
    }

    *timer /= BENCH_NUM_OF_RUNS * n;
    *n_cmp /= BENCH_NUM_OF_RUNS * n;
    avl_free(&avl);
    return NEW_SUCCESS();
}

error_t bench_ht_remove(size_t n, size_t cap, long long *timer, size_t *n_cmp)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;

    mystring_t *arr = get_random_strings(n);
    if (!arr)
        return NEW_ERROR("ошибка создания массива строк", BENCH_ALLOC_ERROR);

    error_t err = NEW_SUCCESS();
    ht_t *ht = ht_new(cap, ht_additive_method);
    for (size_t i = 0; i < n; i++)
        if (is_failure(err = ht_insert(&ht, arr[i])))
        {
            free(arr);
            return err;
        }

    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        for (size_t i = 0; i < n; i++)
        {
            long long int cur_timer = 0;
            size_t cur_cmp = 0;
            BENCH_CHECK_TIME(err = ht_remove(&ht, arr[n - 1], &cur_cmp), cur_timer);
            *n_cmp += cur_cmp;
            *timer += cur_timer;
            if (is_failure(err))
            {
                ht_free(&ht);
                free(arr);
                return err;
            }

            if (is_failure(err = ht_insert(&ht, arr[n - 1])))
            {
                free(arr);
                return err;
            }
        }
    }

    *timer /= BENCH_NUM_OF_RUNS * n;
    *n_cmp /= BENCH_NUM_OF_RUNS * n;
    ht_free(&ht);
    return NEW_SUCCESS();
}

error_t bench_ht_search(size_t n, size_t cap, long long *timer, size_t *n_cmp)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;

    mystring_t *arr = get_random_strings(n);
    if (!arr)
        return NEW_ERROR("ошибка создания массива строк", BENCH_ALLOC_ERROR);

    error_t err = NEW_SUCCESS();

    ht_t *ht = ht_new(cap, ht_additive_method);
    for (size_t i = 0; i < n; i++)
        if (is_failure(err = ht_insert(&ht, arr[i])))
        {
            free(arr);
            return err;
        }

    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        for (size_t i = 0; i < n; i++)
        {
            long long int cur_timer = 0;
            ht_element_t *found = NULL;
            size_t cur_cmp = 0;
            BENCH_CHECK_TIME(err = ht_search(&ht, arr[n - 1], &found, &cur_cmp), cur_timer);
            *n_cmp += cur_cmp;
            *timer += cur_timer;
            if (is_failure(err) && err.code != HT_NOT_FOUND_ERROR)
            {
                free(arr);
                ht_free(&ht);
                return err;
            }
        }
    }

    *timer /= BENCH_NUM_OF_RUNS * n;
    *n_cmp /= BENCH_NUM_OF_RUNS * n;
    ht_free(&ht);
    return NEW_SUCCESS();
}

error_t bench_file_remove(size_t n, long long *timer, size_t *n_cmp)
{
    if (!timer || !n_cmp)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;

    mystring_t *arr = get_random_strings(n);
    if (!arr)
        return NEW_ERROR("ошибка создания массива строк", BENCH_ALLOC_ERROR);

    error_t err = NEW_SUCCESS();

    mystring_t filename;
    if (is_failure(err = new_tmp_filename(filename)))
    {
        free(arr);
        return err;
    }
    for (size_t i = 0; i < n; i++)
        if (is_failure(err = file_insert(filename, arr[i])))
        {
            free(arr);
            return err;
        }

    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        for (size_t i = 0; i < n; i++)
        {
            long long int cur_timer = 0;
            size_t cur_cmp = 0;
            BENCH_CHECK_TIME(err = file_remove(filename, arr[n - 1], &cur_cmp), cur_timer);
            *n_cmp += cur_cmp;
            *timer += cur_timer;
            if (is_failure(err))
            {
                free(arr);
                return err;
            }

            if (is_failure(err = file_insert(filename, arr[n - 1])))
            {
                free(arr);
                return err;
            }
        }
    }

    free(arr);
    *timer /= BENCH_NUM_OF_RUNS * n;
    *n_cmp /= BENCH_NUM_OF_RUNS * n;
    return NEW_SUCCESS();
}

error_t bench_file_search(size_t n, long long *timer, size_t *n_cmp)
{
    if (!timer)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *timer = 0;

    mystring_t *arr = get_random_strings(n);
    if (!arr)
        return NEW_ERROR("ошибка создания массива строк", BENCH_ALLOC_ERROR);

    error_t err = NEW_SUCCESS();

    mystring_t filename;
    if (is_failure(err = new_tmp_filename(filename)))
    {
        free(arr);
        return err;
    }
    for (size_t i = 0; i < n; i++)
        if (is_failure(err = file_insert(filename, arr[i])))
        {
            free(arr);
            return err;
        }

    for (size_t k = 0; k < BENCH_NUM_OF_RUNS; k++)
    {
        for (size_t i = 0; i < n; i++)
        {
            long long int cur_timer = 0;
            size_t cur_cmp = 0;
            BENCH_CHECK_TIME(err = file_search(filename, arr[n - 1], &cur_cmp), cur_timer);
            *timer += cur_timer;
            *n_cmp += cur_cmp;
            if (is_failure(err) && err.code != HT_NOT_FOUND_ERROR)
            {
                free(arr);
                return err;
            }
        }
    }

    *timer /= BENCH_NUM_OF_RUNS * n;
    *n_cmp /= BENCH_NUM_OF_RUNS * n;
    return NEW_SUCCESS();
}

error_t bench_bst_size(size_t n, size_t *size)
{
    if (!size)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *size = n * sizeof(bst_t);
    return NEW_SUCCESS();
}

error_t bench_avl_size(size_t n, size_t *size)
{
    if (!size)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *size = n * sizeof(avl_t);
    return NEW_SUCCESS();
}

error_t bench_ht_size(size_t n, size_t cap, size_t *size)
{
    if (!size)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *size = 0;
    *size += sizeof(ht_t);
    *size += cap * sizeof(ht_element_t *);
    *size += n * sizeof(ht_element_t);

    return NEW_SUCCESS();
}

error_t bench_file_size(size_t n, size_t *size)
{
    if (!size)
        return NEW_ERROR("передан нулевой указатель", BENCH_ARGS_ERROR);

    *size = n * sizeof(mystring_t);
    return NEW_SUCCESS();
}

int64_t bench_tick(void)
{
    int64_t virtual_timer_value;
    __asm__ volatile("mrs %0, cntvct_el0" : "=r"(virtual_timer_value));
    return virtual_timer_value;
}

