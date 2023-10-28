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

size_t input_array(int *begin, int *end)
{
    int *el;
    for (el = begin; el != end; el++)
        if (scanf("%d", el) != 1)
            return el - begin;

    return el - begin;
}

int max_sum(int *begin, int *end)
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
    if (input_array(arr, arr + n) != n)
    {
        error("invalid input");
        return exit_invalid_input;
    }

    int result = max_sum(arr, arr + n);
    printf("Result: %d\n", result);
    return exit_success;
}