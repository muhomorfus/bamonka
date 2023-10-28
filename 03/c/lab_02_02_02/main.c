#include <stdio.h>
#include <stdlib.h>

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

size_t output_array(int *restrict arr, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++)
        if (printf("%d ", arr[i]) < 0)
            return i;

    return i;
}

int same_prefix_and_suffix_digits(int n)
{
    int prefix = n % 10;
    int suffix = n % 10;

    while (n != 0)
    {
        prefix = n % 10;
        n /= 10;
    }

    return prefix == suffix;
}

size_t copy_numbers(int *restrict src, size_t n, int *restrict dst)
{
    size_t last = 0;

    for (size_t i = 0; i < n; i++)
        if (same_prefix_and_suffix_digits(src[i]))
            dst[last++] = src[i];

    return last;
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

    int arr_new[N_MAX];
    int n_new;
    n_new = copy_numbers(arr, n, arr_new);

    if (n_new <= 0)
    {
        error("array must have at least one element "
            "which starts and ends by one digit");
        return exit_empty_array;
    }

    printf("Result: ");
    output_array(arr_new, n_new);
    printf("\n");
    return exit_success;
}