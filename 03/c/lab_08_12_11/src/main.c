#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "doubles_matrix.h"
#include "error.h"
#include "io.h"

#define ARGS_ERROR 2

int main(int argc, char **argv)
{
    if (argc < 2)
        return ARGS_ERROR;

    error_t err = NEW_SUCCESS();
    if (!strcmp(argv[1], "a"))
    {
        if (argc != 5)
            return ARGS_ERROR;

        FILE *f_in1 = NULL, *f_in2 = NULL;
        FILE *f_out = NULL;
        if (!is_failure(err))
            err = io_open_file(&f_in1, argv[2], "r");
        if (!is_failure(err))
            err = io_open_file(&f_in2, argv[3], "r");
        if (!is_failure(err))
            err = io_open_file(&f_out, argv[4], "w");

        doubles_matrix_t a = { 0 };
        doubles_matrix_t b = { 0 };

        if (!is_failure(err))
            err = io_create_matrix_from_f(&a, f_in1);

        if (!is_failure(err))
            err = io_create_matrix_from_f(&b, f_in2);

        doubles_matrix_t c = { 0 };
        if (!is_failure(err))
            err = dm_init(&c, a.rows, a.cols);

        if (!is_failure(err))
            err = dm_fill_addition(&c, &a, &b);

        if (!is_failure(err))
            err = io_print_two_numbers(f_out, c.rows, c.cols);

        if (!is_failure(err))
            err = dm_print(&c, f_out);

        io_close_file(f_in1);
        io_close_file(f_in2);
        io_close_file(f_out);
        dm_free(&a);
        dm_free(&b);
        dm_free(&c);
    }
    else if (!strcmp(argv[1], "m"))
    {
        if (argc != 5)
            return ARGS_ERROR;

        FILE *f_in1 = NULL, *f_in2 = NULL;
        FILE *f_out = NULL;
        if (!is_failure(err))
            err = io_open_file(&f_in1, argv[2], "r");
        if (!is_failure(err))
            err = io_open_file(&f_in2, argv[3], "r");
        if (!is_failure(err))
            err = io_open_file(&f_out, argv[4], "w");

        doubles_matrix_t a = { 0 };
        doubles_matrix_t b = { 0 };

        if (!is_failure(err))
            err = io_create_matrix_from_f(&a, f_in1);

        if (!is_failure(err))
            err = io_create_matrix_from_f(&b, f_in2);

        doubles_matrix_t c = { 0 };
        if (!is_failure(err))
            err = dm_init(&c, a.rows, b.cols);

        if (!is_failure(err))
            err = dm_fill_multiply(&c, &a, &b);

        if (!is_failure(err))
            err = io_print_two_numbers(f_out, c.rows, c.cols);

        if (!is_failure(err))
            err = dm_print(&c, f_out);

        io_close_file(f_in1);
        io_close_file(f_in2);
        io_close_file(f_out);
        dm_free(&a);
        dm_free(&b);
        dm_free(&c);
    }
    else if (!strcmp(argv[1], "o"))
    {
        if (argc != 4)
            return ARGS_ERROR;

        FILE *f_in = NULL;
        FILE *f_out = NULL;
        if (!is_failure(err))
            err = io_open_file(&f_in, argv[2], "r");
        if (!is_failure(err))
            err = io_open_file(&f_out, argv[3], "w");

        doubles_matrix_t a = { 0 };

        if (!is_failure(err))
            err = io_create_matrix_from_f(&a, f_in);

        double det;
        if (!is_failure(err))
            err = dm_det(&a, &det);

        if (!is_failure(err))
            fprintf(f_out, "%.6lf", det);

        io_close_file(f_in);
        io_close_file(f_out);
        dm_free(&a);
    }
    else
        return ARGS_ERROR;

    return err.code;
}
