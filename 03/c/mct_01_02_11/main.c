#define __USE_MINGW_ANSI_STDIO 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NMAX 1024

bool is_square(int n)
{
    if (n < 0 || n % 4 > 1)
        return false;

    if (n == 0 || n == 1)
        return true;
    
    int left = 0;
    int right = n / 2 + 1;

    while (right - left > 1)
    {
        int middle = (left + right) / 2;
        int middle_squared = middle * middle;
        
        if (middle_squared > n)
            right = middle;
        else if (middle_squared < n)
            left = middle;
        else
            return true;
    }

    return false;
}

void delete_element(int *restrict arr, size_t *n, size_t index)
{
    if (*n < index || *n <= 0)
        return;

    for (size_t i = index; i < *n - 1; i++)
        arr[i] = arr[i + 1];

    (*n)--;
}

size_t move_squares(int *restrict src, size_t *n, int *restrict dst)
{
    size_t j = 0;

    for (size_t i = 0; i < *n; i++)
        if (is_square(src[i]))
        {
            dst[j++] = src[i];
            delete_element(src, n, i);
            i--;
        }

    return j;
}

size_t input_array(int *restrict arr, size_t n)
{
    size_t i;

    for (i = 0; i < n; i++)
        if (scanf("%d", &arr[i]) != 1)
            return i;
    
    return i;
}

void output_array(int *restrict arr, size_t n)
{
    for (size_t i = 0; i < n; i++)
        printf("%d ", arr[i]);
}


int main(void)
{
    setbuf(stdout, NULL);
    int a[NMAX];
    int b[NMAX];
    size_t a_size;

    printf("Enter size: ");
    if (scanf("%zu", &a_size) != 1 || a_size == 0 || a_size > NMAX)
    {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }

    printf("Enter array: ");
    if (input_array(a, a_size) != a_size)
    {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }

    size_t b_size = move_squares(a, &a_size, b);
    printf("A: ");
    output_array(a, a_size);
    printf("\n");
    
    printf("B: ");
    output_array(b, b_size);
    printf("\n");

    return EXIT_SUCCESS;
}