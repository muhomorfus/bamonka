#ifndef INTS_MATRIX_H
#define INTS_MATRIX_H

#include <stdlib.h>
#include <stdio.h>

#include "error.h"

#define IM_ALLOC_ERROR 100
#define IM_ARGS_ERROR 101
#define IM_OUT_OF_RANGE_ERROR 102
#define IM_POINTER_ERROR 103
#define IM_READ_ERROR 104
#define IM_PRINT_ERROR 105
#define IM_ARITHMETIC_ERROR 106

typedef struct ints_matrix_t
{
    int **ptr;
    size_t cols;
    size_t rows;
    size_t allocated_cols;
    size_t allocated_rows;
} ints_matrix_t;

error_t im_allocate(ints_matrix_t *m, size_t rows, size_t cols);
error_t im_init(ints_matrix_t *m, size_t rows, size_t cols);
void im_free(ints_matrix_t *m);

error_t im_fill_identity(ints_matrix_t *m);
error_t im_fill_multiply(ints_matrix_t *m, ints_matrix_t *a, ints_matrix_t *b);
error_t im_fill_by_matrix(ints_matrix_t *m, ints_matrix_t *src);
error_t im_fill_from_file(ints_matrix_t *m, FILE *f);
error_t im_print(ints_matrix_t *m, FILE *f);

error_t im_delete_col(ints_matrix_t *m, size_t col);
error_t im_delete_row(ints_matrix_t *m, size_t row);

error_t im_max_element_pos(ints_matrix_t *m, size_t *imax, size_t *jmax);

error_t im_make_square(ints_matrix_t *m);
error_t im_increase_to_size(ints_matrix_t *m, size_t rows, size_t cols);

#endif //INTS_MATRIX_H
