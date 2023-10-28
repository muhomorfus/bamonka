#include <stdio.h>

#define N_MAX 1024
#define N 10

#ifndef N
#define N N_MAX
#endif

#if N > N_MAX
#error N must be less than N_MAX
#endif

typedef enum
{
    exit_success = 0,
    exit_failure = 1,
    exit_invalid_input = 2,
    exit_incorrect_size = 3,
    exit_empty_array = 4,
} return_code;

int error(char *restrict message)
{
    return printf("[ERROR] %s\n", message);
}

size_t input_array(int *restrict arr, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++)
        if (scanf("%d", &arr[i]) != 1)
            return i;

    return i;
}

size_t copy_negatives(int *restrict src, size_t n, int *restrict dst)
{
    size_t last = 0;

    for (size_t i = 0; i < n; i++)
        if (src[i] < 0)
            dst[last++] = src[i];

    return last;
}

double average(int *restrict arr, size_t n)
{
    double sum = 0.0;
    for (size_t i = 0; i < n; i++)
        sum += arr[i];

    return sum / n;
}

int main(void)
{
    int arr[N_MAX];
    size_t n;

    printf("Enter size of array > ");
    if (scanf("%zu", &n) != 1)
    {
        error("invalid input");
        return exit_invalid_input;
    }
    if (n == 0 || n > N)
    {
        error("array must have from one to ten elements");
        return exit_incorrect_size;
    }

    printf("Enter array > ");
    if (input_array(arr, n) != n)
    {
        error("invalid input");
        return exit_invalid_input;
    }

    int arr_neg[N_MAX];
    int n_neg;
    n_neg = copy_negatives(arr, n, arr_neg);

    if (n_neg <= 0)
    {
        error("array must have at least one negative number");
        return exit_empty_array;
    }

    double avg = average(arr_neg, n_neg);
    printf("%.6lf\n", avg);
    return exit_success;
}