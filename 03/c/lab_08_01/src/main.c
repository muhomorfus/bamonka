#include <stdio.h>
#include <stdlib.h>

#include "ints_matrix.h"
#include "error.h"

#define READ_ERROR 1
#define POINTER_ERROR 2

error_t read_two_numbers(size_t *a, size_t *b)
{
    if (!a || !b)
        return NEW_ERROR("pointer is null", POINTER_ERROR);

    if (scanf("%zu%zu", a, b) != 2)
        return NEW_ERROR("failed read", READ_ERROR);

    return NEW_SUCCESS();
}

int main(void)
{
    ints_matrix_t a = { 0 };
    size_t n, m;

    ints_matrix_t b = { 0 };
    size_t p, q;

    error_t err = NEW_SUCCESS();

    err = read_two_numbers(&n, &m);
    if (!is_failure(err))
        err = im_init(&a, n, m);

    if (!is_failure(err))
        err = im_fill_from_file(&a, stdin);

    if (!is_failure(err))
        err = read_two_numbers(&p, &q);

    if (!is_failure(err))
        err = im_init(&b, p, q);

    if (!is_failure(err))
        err = im_fill_from_file(&b, stdin);

    if (!is_failure(err))
        err = im_make_square(&a);

    if (!is_failure(err))
        err = im_make_square(&b);

    size_t size = a.rows;
    if (b.rows > size)
        size = b.rows;

    if (!is_failure(err))
        err = im_increase_to_size(&a, size, size);

    if (!is_failure(err))
        err = im_increase_to_size(&b, size, size);

    ints_matrix_t c = { 0 };
    if (!is_failure(err))
        err = im_init(&c, size, size);

    if (!is_failure(err))
        err = im_fill_identity(&c);

    size_t ro = 0, yamma = 0;
    if (!is_failure(err))
        err = read_two_numbers(&ro, &yamma);

    for (size_t i = 0; i < ro && !is_failure(err); i++)
        err = im_fill_multiply(&c, &c, &a);

    for (size_t i = 0; i < yamma && !is_failure(err); i++)
        err = im_fill_multiply(&c, &c, &b);

    if (!is_failure(err))
        err = im_print(&c, stdout);

    im_free(&a);
    im_free(&b);
    im_free(&c);

    return err.code;
}
