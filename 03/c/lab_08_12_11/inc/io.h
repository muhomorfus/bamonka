#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "doubles_matrix.h"

#define IO_READ_ERROR 11
#define IO_POINTER_ERROR 12
#define IO_FILE_OPEN_ERROR 13
#define IO_WRITE_ERROR 14
#define IO_ARGS_ERROR 15

error_t io_read_two_numbers(FILE *f, size_t *a, size_t *b);
error_t io_print_two_numbers(FILE *f, size_t a, size_t b);
error_t io_open_file(FILE **f, char *name, char *mode);
void io_close_file(FILE *f);
error_t io_create_matrix_from_f(doubles_matrix_t *m, FILE *f);

#endif //IO_H
