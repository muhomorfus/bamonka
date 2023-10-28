#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NMAX 128
#define N 10

#if N > NMAX
#error Max size must be bigger than size
#endif

typedef enum
{
    exit_success = 0,
    exit_failure = 1,
    exit_invalid_input = 2,
    exit_empty = 3,
    exit_too_big = 4,
} return_code;

typedef int array_t[NMAX];
typedef array_t matrix_t[NMAX];

void error(char *message)
{
    printf("[ERROR] %s\n", message);
}

size_t print_matrix(matrix_t matrix, size_t n, size_t m)
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
            if (printf("%d ", matrix[i][j]) < 0)
                return i * n + j;
        printf("\n");
    }
    return n * m;
}

size_t input_matrix(matrix_t matrix, size_t n, size_t m)
{
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            if (scanf("%d", &matrix[i][j]) != 1)
                return i * m + j;
    return n * m;
}

int multiplication_array_elements(array_t array, size_t m)
{
    int result = 1;
    for (size_t j = 0; j < m; j++)
        result *= array[j];

    return result;
}

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void swap_arrays(array_t array1, array_t array2, size_t m)
{
    for (size_t j = 0; j < m; j++)
        swap(&array1[j], &array2[j]);
}

void sort(matrix_t matrix, size_t n, size_t m)
{
    for (size_t i = 0; i < n - 1; i++)
        for (size_t j = 0; j < n - i - 1; j++)
        {
            int mul_current = multiplication_array_elements(matrix[j], m);
            int mul_next = multiplication_array_elements(matrix[j + 1], m);
            if (mul_current > mul_next)
                swap_arrays(matrix[j], matrix[j + 1], m);
        }
}

int main(void)
{
    size_t n, m;
    matrix_t matrix;

    printf("Enter size of matrix > ");
    if (scanf("%zu%zu", &n, &m) != 2)
    {
        error("invalid input");
        return exit_invalid_input;
    }
    if (n > N || m > N)
    {
        error("sizes are too big");
        return exit_too_big;
    }

    if (n == 0 || m == 0)
    {
        error("matrix size must me positive integer");
        return exit_empty;
    }

    printf("Enter matrix:\n");
    if (input_matrix(matrix, n, m) != n * m)
    {
        error("invalid input");
        return exit_invalid_input;
    }

    sort(matrix, n, m);
    printf("Result:\n");
    print_matrix(matrix, n, m);
    printf("\n");
    return exit_success;
}