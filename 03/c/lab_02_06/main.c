#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>

#define N_MAX 100000
#define N 100000

#ifndef N
#define N N_MAX
#endif

#if N > N_MAX
#error N must be less than N_MAX
#endif

typedef enum
{
    exit_success,
    exit_failure,
} return_code;

int error(char *restrict message)
{
    return printf("[ERROR] %s\n", message);
}

size_t input_array(int *begin, int *end)
{
    int *el;
    for (el = begin; el != end; el++)
        if (scanf("%d", el) != 1)
            return el - begin;

    return el - begin;
}

int count_max_sum_1(int *arr, size_t n)
{
    int first = 0;
    int second = n - 1;

    int current_sum = arr[first] + arr[second];
    int max_sum = current_sum;

    while (first <= second)
    {
        current_sum = arr[first] + arr[second];
        if (current_sum > max_sum)
            max_sum = current_sum;
        first++;
        second--;
    }

    return max_sum;
}

int count_max_sum_2(int *arr, size_t n)
{
    int first = 0;
    int second = n - 1;

    int current_sum = *(arr + first) + *(arr + second);
    int max_sum = current_sum;

    while (first <= second)
    {
        current_sum = *(arr + first) + *(arr + second);
        if (current_sum > max_sum)
            max_sum = current_sum;
        first++;
        second--;
    }

    return max_sum;
}

int count_max_sum_3(int *begin, int *end)
{
    int *first = begin;
    int *second = end - 1;

    int current_sum = *first + *second;
    int max_sum = current_sum;

    while (first <= second)
    {
        current_sum = *first + *second;
        if (current_sum > max_sum)
            max_sum = current_sum;
        first++;
        second--;
    }

    return max_sum;
}

int64_t process_1(int *arr, size_t n)
{
    struct timeval tv_start, tv_stop;
    int64_t elapsed_time;
    gettimeofday(&tv_start, NULL);
    count_max_sum_1(arr, n);
    gettimeofday(&tv_stop, NULL);
    elapsed_time = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL +
    (tv_stop.tv_usec - tv_start.tv_usec);

    return elapsed_time;
}

int64_t process_2(int *arr, size_t n)
{
    struct timeval tv_start, tv_stop;
    int64_t elapsed_time;
    gettimeofday(&tv_start, NULL);
    count_max_sum_2(arr, n);
    gettimeofday(&tv_stop, NULL);
    elapsed_time = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL +
    (tv_stop.tv_usec - tv_start.tv_usec);

    return elapsed_time;
}

int64_t process_3(int *arr, size_t n)
{
    struct timeval tv_start, tv_stop;
    int64_t elapsed_time;
    gettimeofday(&tv_start, NULL);
    count_max_sum_3(arr, arr + n);
    gettimeofday(&tv_stop, NULL);
    elapsed_time = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL +
    (tv_stop.tv_usec - tv_start.tv_usec);

    return elapsed_time;
}

int main(void)
{
    int arr[N_MAX];
    size_t n;

    scanf("%zu", &n);
    input_array(arr, arr + n);

    int64_t time1 = process_1(arr, n);
    int64_t time2 = process_2(arr, n);
    int64_t time3 = process_3(arr, n);

    printf("%"PRId64" %"PRId64" %"PRId64"\n", time1, time2, time3);
    return exit_success;
}