#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NMAX 128
#define N 10

#define VALUE -1
#define NEED 2

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

void copy(array_t src, array_t dst, size_t m)
{
    for (size_t j = 0; j < m; j++)
        dst[j] = src[j];
}

void shift_arrays(matrix_t matrix, size_t *n, size_t *m, size_t from_pos)
{
    for (size_t i = *n; i > from_pos; i--)
        copy(matrix[i - 1], matrix[i], *m);
    (*n)++;
}

void insert_array(matrix_t matrix, size_t *n, size_t *m, array_t array, size_t pos)
{
    shift_arrays(matrix, n, m, pos);
    copy(array, matrix[pos], *m);
}

void fill_array(array_t array, size_t m, int value)
{
    for (size_t j = 0; j < m; j++)
        array[j] = value;
}

int sum_of_digits(int number)
{
    int sum = 0;

    while (number != 0)
    {
        sum += abs(number % 10);
        number /= 10;
    }

    return sum;
}

int count_numbers_with_odd_sum(array_t array, size_t m)
{
    int counter = 0;
    for (size_t j = 0; j < m; j++)
        counter += sum_of_digits(array[j]) % 2 != 0;

    return counter;
}

void insert_arrays(matrix_t matrix, size_t *n, size_t *m, int value, int need)
{
    array_t array;
    fill_array(array, *m, value);

    for (size_t i = 0; i < *n; i++)
        if (count_numbers_with_odd_sum(matrix[i], *m) >= need)
            insert_array(matrix, n, m, array, i++);
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

    insert_arrays(matrix, &n, &m, VALUE, NEED);
    printf("Result:\n");
    print_matrix(matrix, n, m);
    printf("\n");
    return exit_success;
}