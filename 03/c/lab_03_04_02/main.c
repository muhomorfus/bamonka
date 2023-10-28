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
    exit_matrix_non_square = 5,
    exit_no_answer = 6,
} return_code;

typedef int array_t[NMAX];
typedef array_t matrix_t[NMAX];

void error(char *message)
{
    printf("[ERROR] %s\n", message);
}

size_t input_matrix(matrix_t matrix, size_t n, size_t m)
{
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            if (scanf("%d", &matrix[i][j]) != 1)
                return i * m + j;
    return n * m;
}

bool ends_with_digit(int number, int digit)
{
    return abs(number % 10) == digit;
}

int find_max_ends_with_digit(matrix_t matrix, size_t n, size_t m, int digit)
{
    int maximum = 0;

    for (size_t i = 0; i < n; i++)
        for (size_t j = n - i; j < m; j++)
            if (ends_with_digit(matrix[i][j], digit))
                if (maximum == 0 || matrix[i][j] > maximum)
                    maximum = matrix[i][j];

    return maximum;
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

    if (n != m)
    {
        error("matrix must be square");
        return exit_matrix_non_square;
    }

    printf("Enter matrix:\n");
    if (input_matrix(matrix, n, m) != n * m)
    {
        error("invalid input");
        return exit_invalid_input;
    }

    int maximum = find_max_ends_with_digit(matrix, n, m, DIGIT);
    if (!ends_with_digit(maximum, DIGIT))
    {
        error("cant find maximum");
        return exit_no_answer;
    }

    printf("Result: %d\n", maximum);
    return exit_success;
}