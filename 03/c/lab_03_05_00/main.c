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
    exit_no_primes = 5,
} return_code;

typedef int array_t[NMAX];
typedef array_t matrix_t[NMAX];
typedef int long_array_t[NMAX * NMAX];

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

bool is_prime(int n)
{
    if (n == 2)
        return true;

    if (n <= 1 || n % 2 == 0)
        return false;

    for (int i = 3; i < n / 2; i += 2)
        if (n % i == 0)
            return false;

    return true;
}

size_t fill_primes_array(matrix_t matrix, size_t n, size_t m, long_array_t primes)
{
    size_t index = 0;

    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            if (is_prime(matrix[i][j]))
            {
                primes[index] = matrix[i][j];
                index++;
            }

    return index;
}

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void reverse(long_array_t arr, size_t n)
{
    for (size_t i = 0; i < n / 2; i++)
        swap(&arr[i], &arr[n - i - 1]);
}

size_t fill_from_array(matrix_t matrix, size_t n, size_t m, long_array_t primes)
{
    size_t index = 0;

    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            if (is_prime(matrix[i][j]))
            {
                matrix[i][j] = primes[index];
                index++;
            }

    return index;
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

    printf("Enter matrix:\n");
    if (input_matrix(matrix, n, m) != n * m)
    {
        error("invalid input");
        return exit_invalid_input;
    }

    long_array_t primes;
    size_t primes_arr_size = fill_primes_array(matrix, n, m, primes);
    if (!primes_arr_size)
    {
        error("there are no primes");
        return exit_no_primes;
    }

    reverse(primes, primes_arr_size);
    fill_from_array(matrix, n, m, primes);

    printf("Result:\n");
    print_matrix(matrix, n, m);
    printf("\n");
    return exit_success;
}