#ifndef DOUBLES_MATRIX_H
#define DOUBLES_MATRIX_H

#include <stdlib.h>
#include <stdio.h>

#include "error.h"

#define DM_ALLOC_ERROR 100
#define DM_ARGS_ERROR 101
#define DM_OUT_OF_RANGE_ERROR 102
#define DM_POINTER_ERROR 103
#define DM_READ_ERROR 104
#define DM_PRINT_ERROR 105
#define DM_ARITHMETIC_ERROR 106

typedef struct doubles_matrix_t
{
    double **ptr;
    size_t cols;
    size_t rows;
    size_t allocated_cols;
    size_t allocated_rows;
} doubles_matrix_t;

error_t dm_allocate(doubles_matrix_t *m, size_t rows, size_t cols);
error_t dm_init(doubles_matrix_t *m, size_t rows, size_t cols);
void dm_free(doubles_matrix_t *m);

error_t dm_fill_multiply(doubles_matrix_t *m, doubles_matrix_t *a, doubles_matrix_t *b);
error_t dm_fill_addition(doubles_matrix_t *m, doubles_matrix_t *a, doubles_matrix_t *b);
error_t dm_fill_by_matrix(doubles_matrix_t *m, doubles_matrix_t *src);
error_t dm_fill_from_file(doubles_matrix_t *m, FILE *f);
error_t dm_print(doubles_matrix_t *m, FILE *f);

error_t dm_det(doubles_matrix_t *m, double *det);

#endif //DOUBLES_MATRIX_H
