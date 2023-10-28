#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "array.h"

int f_count_ints(FILE *f, size_t *n)
{
    if (!f)
        return ERROR_NULL_POINTER;

    *n = 0;
    int buf;
    while (fscanf(f, "%d", &buf) == 1)
        (*n)++;

    if (!feof(f))
        return ERROR_FILE_READING;

    rewind(f);
    return EXIT_SUCCESS;
}

int f_read_ints_array(FILE *f, int *pb, int *pe)
{
    if (!f || !pb || !pe)
        return ERROR_NULL_POINTER;
    if (pb == pe)
        return ERROR_EMPTY;
    if (pb > pe)
        return ERROR_OVERLAP_POINTERS;

    while (fscanf(f, "%d", pb++) == 1 && pb < pe);

    if (!feof(f))
        return ERROR_FILE_READING;

    return EXIT_SUCCESS;
}

int f_print_ints_array(FILE *f, int *pb, int *pe)
{
    if (!f || !pb || !pe)
        return ERROR_NULL_POINTER;
    if (pb == pe)
        return ERROR_EMPTY;
    if (pb > pe)
        return ERROR_OVERLAP_POINTERS;

    while (pb != pe)
        if (fprintf(f, "%d ", *(pb++)) < 0)
            return ERROR_FILE_WRITING;

    return EXIT_SUCCESS;
}

int new_ints_array(size_t size, int **arr)
{
    if (!arr)
        return ERROR_NULL_POINTER;
    if (!size)
        return ERROR_EMPTY;

    *arr = malloc(size * sizeof(int));
    if (*arr == NULL)
        return ERROR_MALLOC;

    return EXIT_SUCCESS;
}

void free_ints_array(int **arr)
{
    free(*arr);
    *arr = NULL;
}

int count_average(const int *pb, const int *pe, double *avg)
{
    if (!pb || !pe)
        return ERROR_NULL_POINTER;
    if (pb == pe)
        return ERROR_EMPTY;
    if (pb > pe)
        return ERROR_OVERLAP_POINTERS;

    *avg = 0;
    for (const int *p = pb; p < pe; p++)
        *avg += *p;

    *avg /= (pe - pb);
    return EXIT_SUCCESS;
}

int key(const int *pb_src, const int *pe_src, int **pb_dst, int **pe_dst)
{
    if (!pb_src || !pe_src || !pb_dst || !pe_dst || !*pb_dst || !*pe_dst)
        return ERROR_NULL_POINTER;
    if (pb_src == pe_src)
        return ERROR_EMPTY;
    if (pb_src > pe_src)
        return ERROR_OVERLAP_POINTERS;

    int code = 0;
    double average = 0;
    code = count_average(pb_src, pe_src, &average);
    if (code)
    {
        free_ints_array(pb_dst);
        *pb_dst = NULL;
        return code;
    }

    int *f = *pb_dst;
    for (const int *p = pb_src; p < pe_src; p++)
        if ((double) *p > average)
        {
            *(f++) = *p;
            if (f >= *pe_dst)
                return ERROR_OVERFLOW;
        }

    if (f == *pb_dst)
        return ERROR_EMPTY_FILTER;
    *pe_dst = f;
    return EXIT_SUCCESS;
}

int swap(void *a, void *b, size_t size)
{
    if (!a || !b)
        return ERROR_NULL_POINTER;
    if (size == 0)
        return ERROR_EMPTY;
    char *char_a = a;
    char *char_b = b;
    if (char_a < char_b + size && char_a > char_b - size)
        return ERROR_OVERLAP_POINTERS;
    if (char_b < char_a + size && char_b > char_a - size)
        return ERROR_OVERLAP_POINTERS;

    void *buf = malloc(size);
    if (!buf)
        return ERROR_MALLOC;

    memcpy(buf, a, size);
    memcpy(a, b, size);
    memcpy(b, buf, size);

    free(buf);
    return EXIT_SUCCESS;
}

int compare_ints(const void *a, const void *b)
{
    const int *int_a = a;
    const int *int_b = b;
    return *int_a - *int_b;
}

void mysort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
    if (!base || !nmemb || !size || !compar)
        return;

    char *start = base;
    char *end = start + nmemb * size;
    for (char *p = base; p < end; p += size)
    {
        bool sorted = true;

        for (char *pn = base; pn < end - size - p + start; pn += size)
            if ((*compar)(pn, pn + size) > 0)
            {
                swap(pn, pn + size, size);
                sorted = false;
            }
        if (sorted)
            return;
    }
}


