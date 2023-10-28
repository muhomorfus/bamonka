#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "array.h"

int lshift1(int *arr, size_t size)
{
    if (!arr || !size)
        return ARR_ARGS_ERROR;

    int buf = arr[0];
    for (size_t i = 0; i < size - 1; i++)
        arr[i] = arr[i + 1];

    arr[size - 1] = buf;
    return EXIT_SUCCESS;
}

int rshift1(int *arr, size_t size)
{
    if (!arr || !size)
        return ARR_ARGS_ERROR;

    int buf = arr[size - 1];
    for (size_t i = size; i > 0; i--)
        arr[i] = arr[i - 1];

    arr[0] = buf;
    return EXIT_SUCCESS;
}

int arr_lshift(int *arr, size_t size, int shift)
{
    if (!arr || !size)
        return ARR_ARGS_ERROR;

    int err = EXIT_SUCCESS;
    if (shift < 0)
    {
        for (int i = 0; i > shift; i--)
            if ((err = rshift1(arr, size)))
                return err;
    }
    else if (shift > 0)
    {
        for (int i = 0; i < shift; i++)
            if ((err = lshift1(arr, size)))
                return err;
    }

    return EXIT_SUCCESS;
}

bool is_square(int n)
{
    if (n < 0)
        return false;

    if (n == 0 || n == 1)
        return true;

    for (int k = 2; k <= n / 2; k++)
        if (k * k == n)
            return true;

    return false;
}

int arr_copy_squares(int *src, size_t src_size, int *dst, size_t *dst_size)
{
    if (!src || !src_size || !dst_size)
        return ARR_ARGS_ERROR;

    *dst_size = 0;
    for (size_t i = 0; i < src_size; i++)
        if (is_square(src[i]))
        {
            if (dst)
                dst[*dst_size] = src[i];
            (*dst_size)++;
        }

    if (!dst)
        return ARR_NOT_ALLOCATED_ERROR;

    return EXIT_SUCCESS;
}