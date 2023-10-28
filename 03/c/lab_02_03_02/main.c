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

int is_square(int n)
{
    if (n < 0)
        return 0;

    if (n <= 1)
        return 1;

    int left = 0;
    int right = n / 2 + 1;
    int middle;
    int middle_squared;

    while (right - left > 1)
    {
        middle = (left + right) / 2;
        middle_squared = middle * middle;

        if (middle_squared > n)
            right = middle;
        else if (middle_squared == n)
            return 1;
        else
            left = middle;
    }
    return 0;
}

void remove_squares(int *restrict arr, size_t *n)
{
    for (size_t i = 0; i < *n; i++)
        if (is_square(arr[i]))
        {
            for (size_t j = i; j < *n - 1; j++)
                arr[j] = arr[j + 1];

            (*n)--;
            i--;
        }
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

    remove_squares(arr, &n);
    if (n == 0)
    {
        error("array must have at least one non-square element");
        return exit_empty_array;
    }
    printf("Result: ");
    output_array(arr, n);
    printf("\n");
    return exit_success;
}