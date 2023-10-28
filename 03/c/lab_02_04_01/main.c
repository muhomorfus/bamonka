#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    exit_overflow = 100,
} return_code;

int error(char *restrict message)
{
    return printf("[ERROR] %s\n", message);
}

size_t input_array(int *restrict arr)
{
    size_t i = 0;
    while (i <= N && scanf("%d", &arr[i]) == 1)
        i++;

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

void sort(int *restrict arr, size_t n)
{
    for (size_t i = 1; i < n; i++)
    {
        int element = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > element)
        {
            arr[j] = arr[j - 1];
            j--;
        }

        arr[j] = element;
    }
}

int main(void)
{
    int arr[N_MAX];
    size_t n;
    bool is_overflow = false;

    printf("Enter array > ");
    n = input_array(arr);

    if (n == 0)
    {
        error("array must have from one to ten elements");
        return exit_empty_array;
    }
    if (n > N)
    {
        n = N;
        is_overflow = true;
    }

    sort(arr, n);
    printf("Result: ");
    output_array(arr, n);
    printf("\n");

    if (is_overflow)
        return exit_overflow;

    return exit_success;
}