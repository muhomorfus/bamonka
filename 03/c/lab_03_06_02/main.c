#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NMAX 128
#define N 10

#define DIGIT 5

#if N > NMAX
#error Max size must be bigger than size
#endif

typedef enum
{
    exit_success = 0,
    exit_failure = 1,
    exit_invalid_input = 2,
    exit_matrix_empty = 3,
    exit_sizes_too_big = 4,
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

void fill_bull_turn(matrix_t matrix, size_t n, size_t m)
{
    size_t shift = 1;
    size_t start_n = 1;
    if (m % 2 == 0)
    {
        shift = -1;
        start_n = n;
    }

    int value = 1;
    for (size_t j = m; j > 0; j--)
    {
        for (size_t i = start_n; i <= n && i > 0; i += shift)
            matrix[i - 1][j - 1] = value++;

        if (start_n == n)
        {
            start_n = 1;
            shift = 1;
        }
        else
        {
            shift = -1;
            start_n = n;
        }
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
        return exit_sizes_too_big;
    }

    if (n == 0 || m == 0)
    {
        error("matrix size must me positive integer");
        return exit_matrix_empty;
    }

    fill_bull_turn(matrix, n, m);

    printf("Result:\n");
    print_matrix(matrix, n, m);
    printf("\n");
    return exit_success;
}