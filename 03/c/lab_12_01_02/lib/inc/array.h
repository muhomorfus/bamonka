#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <stdio.h>

#define ARR_ARGS_ERROR 100
#define ARR_NOT_ALLOCATED_ERROR 101

int arr_lshift(int *arr, size_t size, int shift);
int arr_copy_squares(int *src, size_t src_size, int *dst, size_t *dst_size);

#endif //ARRAY_H
