#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <stdio.h>

#define ERROR_MALLOC 100
#define ERROR_EMPTY 101
#define ERROR_NULL_POINTER 102
#define ERROR_OVERLAP_POINTERS 103
#define ERROR_EMPTY_FILTER 104
#define ERROR_FILE_READING 105
#define ERROR_FILE_WRITING 106

int f_count_ints(FILE *f, size_t *n);
int f_read_ints_array(FILE *f, int *pb, int *pe);
int f_print_ints_array(FILE *f, int *pb, int *pe);

int new_ints_array(size_t size, int **arr);
void free_ints_array(int **arr);

int key(const int *pb_src, const int *pe_src, int **pb_dst, int **pe_dst);

int compare_ints(const void *a, const void *b);

void mysort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

#endif //ARRAY_H
