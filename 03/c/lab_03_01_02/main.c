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

void error(char *restrict message)
{
    printf("[ERROR] %s\n", message);
}

size_t print_array(array_t arr, size_t n)
{
    for (size_t i = 0; i < n; i++)
        if (printf("%d ", arr[i]) < 0)
            return i;
    return n;
}

size_t input_matrix(matrix_t matrix, size_t n, size_t m)
{
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            if (scanf("%d", &matrix[i][j]) != 1)
                return i * m + j;
    return n * m;
}

bool neg_and_pos_alternating(matrix_t matrix, size_t n, size_t j)
{
    if (n <= 1)
        return false;

    bool negative = false;
    for (size_t i = 0; i < n; i++)
        if (matrix[i][j] == 0)
            return false;
        else if (i == 0)
            negative = matrix[i][j] < 0;
        else if (negative == (matrix[i][j] < 0))
            return false;
        else
            negative = matrix[i][j] < 0;

    return true;
}

size_t gen_arr(matrix_t matrix, size_t n, size_t m, array_t arr)
{
    for (size_t j = 0; j < m; j++)
        arr[j] = neg_and_pos_alternating(matrix, n, j);

    return m;
}

int main(void)
{
    size_t n, m;
    matrix_t matrix;
    array_t arr;

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

    size_t arr_size = gen_arr(matrix, n, m, arr);
    printf("Result: ");
    print_array(arr, arr_size);
    printf("\n");
    return exit_success;
}