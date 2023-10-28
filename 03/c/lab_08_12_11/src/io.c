#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "error.h"
#include "doubles_matrix.h"

error_t io_read_two_numbers(FILE *f, size_t *a, size_t *b)
{
    if (!a || !b)
        return NEW_ERROR("pointer is null", IO_POINTER_ERROR);

    if (fscanf(f, "%zu%zu", a, b) != 2)
        return NEW_ERROR("failed read", IO_READ_ERROR);

    return NEW_SUCCESS();
}

error_t io_print_two_numbers(FILE *f, size_t a, size_t b)
{
    if (!a || !b)
        return NEW_ERROR("pointer is null", IO_POINTER_ERROR);

    if (fprintf(f, "%zu %zu\n", a, b) < 0)
        return NEW_ERROR("failed print", IO_WRITE_ERROR);

    return NEW_SUCCESS();
}

error_t io_open_file(FILE **f, char *name, char *mode)
{
    if (!f)
        return NEW_ERROR("null pointer", IO_ARGS_ERROR);

    *f = fopen(name, mode);
    if (!*f)
        return NEW_ERROR("error with opening file", IO_FILE_OPEN_ERROR);
    return NEW_SUCCESS();
}

error_t io_create_matrix_from_f(doubles_matrix_t *m, FILE *f)
{
    error_t err = NEW_SUCCESS();

    size_t rows, cols;
    if (!is_failure(err))
        err = io_read_two_numbers(f, &rows, &cols);

    if (!is_failure(err))
        err = dm_init(m, rows, cols);

    if (!is_failure(err))
        err = dm_fill_from_file(m, f);

    return err;
}

void io_close_file(FILE *f)
{
    if (f)
        fclose(f);
}