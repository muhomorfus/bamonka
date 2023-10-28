#include <stdlib.h>
#include <check.h>
#include <math.h>

#include "doubles_matrix.h"
#include "error.h"

#define EPS 1e-5

void assert_success(error_t err)
{
    ck_assert_int_eq(is_failure(err), 0);
}

void assert_failure(error_t err)
{
    ck_assert_int_ne(is_failure(err), 0);
}

void assert_double_equals(double a, double b)
{
    ck_assert_int_eq(fabs(a - b) < EPS, 1);
}

void dm_assert_equals(doubles_matrix_t *a, doubles_matrix_t *b)
{
    ck_assert_int_eq(a->rows, b->rows);
    ck_assert_int_eq(a->cols, b->cols);

    for (size_t i = 0; i < a->rows; i++)
        for (size_t j = 0; j < a->cols; j++)
            assert_double_equals(a->ptr[i][j], b->ptr[i][j]);
}

error_t
dm_from_array(doubles_matrix_t *m, double *arr, size_t rows, size_t cols)
{
    error_t err;
    if (is_failure(err = dm_init(m, rows, cols)))
        return err;

    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            m->ptr[i][j] = arr[i * cols + j];

    return NEW_SUCCESS();
}

START_TEST(test_dm_fill_multiply_usual)
    {
        double a_arr[] = { 1, 2, 3, 4, 5, 6 };
        double b_arr[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
        doubles_matrix_t a = { 0 }, b = { 0 }, c = { 0 };
        assert_success(dm_from_array(&a, a_arr, 3, 2));
        assert_success(dm_from_array(&b, b_arr, 2, 4));
        assert_success(dm_init(&c, 3, 4));
        assert_success(dm_fill_multiply(&c, &a, &b));

        double arr[] = { 13, 10, 7, 4, 33, 26, 19, 12, 53, 42, 31, 20 };
        doubles_matrix_t res = { 0 };
        assert_success(dm_from_array(&res, arr, 3, 4));
        dm_assert_equals(&c, &res);
        dm_free(&a);
        dm_free(&b);
        dm_free(&c);
        dm_free(&res);
    }
END_TEST

START_TEST(test_dm_fill_multiply_identity)
    {
        double a_arr[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
        double b_arr[] = { 7, 6, 5, 4, 3, 2 };
        doubles_matrix_t a = { 0 }, b = { 0 }, c = { 0 };
        assert_success(dm_from_array(&a, a_arr, 3, 3));
        assert_success(dm_from_array(&b, b_arr, 3, 2));
        assert_success(dm_init(&c, 3, 2));
        assert_success(dm_fill_multiply(&c, &a, &b));

        double arr[] = { 7, 6, 5, 4, 3, 2 };
        doubles_matrix_t res = { 0 };
        assert_success(dm_from_array(&res, arr, 3, 2));
        dm_assert_equals(&c, &res);
        dm_free(&a);
        dm_free(&b);
        dm_free(&c);
        dm_free(&res);
    }
END_TEST

START_TEST(test_dm_fill_multiply_invalid_sizes)
    {
        double a_arr[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
        double b_arr[] = { 7, 6, 5, 4 };
        doubles_matrix_t a = { 0 }, b = { 0 }, c = { 0 };
        assert_success(dm_from_array(&a, a_arr, 3, 3));
        assert_success(dm_from_array(&b, b_arr, 2, 2));
        assert_success(dm_init(&c, 3, 4));
        assert_failure(dm_fill_multiply(&c, &a, &b));

        dm_free(&a);
        dm_free(&b);
        dm_free(&c);
    }
END_TEST

START_TEST(test_dm_fill_multiply_not_initialized)
    {
        doubles_matrix_t a = { 0 }, b = { 0 }, c = { 0 };
        assert_failure(dm_fill_multiply(&c, &a, &b));
    }
END_TEST

START_TEST(test_dm_fill_multiply_null_pointers)
    {
        assert_failure(dm_fill_multiply(NULL, NULL, NULL));
    }
END_TEST

Suite *dm_fill_multiply_suite(Suite *s)
{
    TCase *tc_neg, *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_dm_fill_multiply_invalid_sizes);
    tcase_add_test(tc_neg, test_dm_fill_multiply_not_initialized);
    tcase_add_test(tc_neg, test_dm_fill_multiply_null_pointers);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_dm_fill_multiply_usual);
    tcase_add_test(tc_pos, test_dm_fill_multiply_identity);

    suite_add_tcase(s, tc_neg);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_dm_fill_by_matrix_usual)
    {
        double b_arr[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
        doubles_matrix_t a = { 0 }, b = { 0 };
        assert_success(dm_init(&a, 2, 2));
        assert_success(dm_from_array(&b, b_arr, 2, 4));
        assert_success(dm_fill_by_matrix(&a, &b));

        double arr[] = { 7, 6, 3, 2 };
        doubles_matrix_t res = { 0 };
        assert_success(dm_from_array(&res, arr, 2, 2));
        dm_assert_equals(&a, &res);
        dm_free(&a);
        dm_free(&b);
        dm_free(&res);
    }
END_TEST

START_TEST(test_dm_fill_by_matrix_eq_sizes)
    {
        double b_arr[] = { 7, 6, 5, 4 };
        doubles_matrix_t a = { 0 }, b = { 0 };
        assert_success(dm_init(&a, 2, 2));
        assert_success(dm_from_array(&b, b_arr, 2, 2));
        assert_success(dm_fill_by_matrix(&a, &b));

        double arr[] = { 7, 6, 5, 4 };
        doubles_matrix_t res = { 0 };
        assert_success(dm_from_array(&res, arr, 2, 2));
        dm_assert_equals(&a, &res);
        dm_free(&a);
        dm_free(&b);
        dm_free(&res);
    }
END_TEST

START_TEST(test_dm_fill_by_matrix_invalid_num_of_cols)
    {
        double b_arr[] = { 7, 6, 5, 4 };
        doubles_matrix_t a = { 0 }, b = { 0 };
        assert_success(dm_init(&a, 2, 2));
        assert_success(dm_from_array(&b, b_arr, 2, 1));
        assert_failure(dm_fill_by_matrix(&a, &b));

        dm_free(&a);
        dm_free(&b);
    }
END_TEST

START_TEST(test_dm_fill_by_matrix_invalid_num_of_rows)
    {
        double b_arr[] = { 7, 6, 5, 4 };
        doubles_matrix_t a = { 0 }, b = { 0 };
        assert_success(dm_init(&a, 2, 2));
        assert_success(dm_from_array(&b, b_arr, 1, 2));
        assert_failure(dm_fill_by_matrix(&a, &b));

        dm_free(&a);
        dm_free(&b);
    }
END_TEST

START_TEST(test_dm_fill_by_matrix_not_initialized)
    {
        doubles_matrix_t a = { 0 }, b = { 0 };
        assert_failure(dm_fill_by_matrix(&a, &b));
    }
END_TEST

START_TEST(test_dm_fill_by_matrix_null_pointers)
    {
        assert_failure(dm_fill_by_matrix(NULL, NULL));
    }
END_TEST

Suite *dm_fill_by_matrix_suite(Suite *s)
{
    TCase *tc_neg, *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_dm_fill_by_matrix_invalid_num_of_cols);
    tcase_add_test(tc_neg, test_dm_fill_by_matrix_invalid_num_of_rows);
    tcase_add_test(tc_neg, test_dm_fill_by_matrix_not_initialized);
    tcase_add_test(tc_neg, test_dm_fill_by_matrix_null_pointers);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_dm_fill_by_matrix_usual);
    tcase_add_test(tc_pos, test_dm_fill_by_matrix_eq_sizes);

    suite_add_tcase(s, tc_neg);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_dm_fill_addition_usual)
    {
        double a_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        double b_arr[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
        doubles_matrix_t a = { 0 }, b = { 0 }, c = { 0 };
        assert_success(dm_from_array(&a, a_arr, 4, 2));
        assert_success(dm_from_array(&b, b_arr, 4, 2));
        assert_success(dm_init(&c, 4, 2));
        assert_success(dm_fill_addition(&c, &a, &b));

        double arr[] = { 8, 8, 8, 8, 8, 8, 8, 8 };
        doubles_matrix_t res = { 0 };
        assert_success(dm_from_array(&res, arr, 4, 2));
        dm_assert_equals(&c, &res);
        dm_free(&a);
        dm_free(&b);
        dm_free(&c);
        dm_free(&res);
    }
END_TEST

START_TEST(test_dm_fill_addition_zeros)
    {
        double a_arr[] = { 0, 0, 0, 0, 0, 0 };
        double b_arr[] = { 7, 6, 5, 4, 3, 2 };
        doubles_matrix_t a = { 0 }, b = { 0 }, c = { 0 };
        assert_success(dm_from_array(&a, a_arr, 3, 2));
        assert_success(dm_from_array(&b, b_arr, 3, 2));
        assert_success(dm_init(&c, 3, 2));
        assert_success(dm_fill_addition(&c, &a, &b));

        double arr[] = { 7, 6, 5, 4, 3, 2 };
        doubles_matrix_t res = { 0 };
        assert_success(dm_from_array(&res, arr, 3, 2));
        dm_assert_equals(&c, &res);
        dm_free(&a);
        dm_free(&b);
        dm_free(&c);
        dm_free(&res);
    }
END_TEST

START_TEST(test_dm_fill_addition_invalid_sizes)
    {
        double a_arr[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
        double b_arr[] = { 7, 6, 5, 4 };
        doubles_matrix_t a = { 0 }, b = { 0 }, c = { 0 };
        assert_success(dm_from_array(&a, a_arr, 3, 3));
        assert_success(dm_from_array(&b, b_arr, 2, 2));
        assert_success(dm_init(&c, 3, 4));
        assert_failure(dm_fill_addition(&c, &a, &b));

        dm_free(&a);
        dm_free(&b);
        dm_free(&c);
    }
END_TEST

START_TEST(test_dm_fill_addition_not_initialized)
    {
        doubles_matrix_t a = { 0 }, b = { 0 }, c = { 0 };
        assert_failure(dm_fill_addition(&c, &a, &b));
    }
END_TEST

START_TEST(test_dm_fill_addition_null_pointers)
    {
        assert_failure(dm_fill_addition(NULL, NULL, NULL));
    }
END_TEST

Suite *dm_fill_addition_suite(Suite *s)
{
    TCase *tc_neg, *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_dm_fill_addition_invalid_sizes);
    tcase_add_test(tc_neg, test_dm_fill_addition_not_initialized);
    tcase_add_test(tc_neg, test_dm_fill_addition_null_pointers);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_dm_fill_addition_usual);
    tcase_add_test(tc_pos, test_dm_fill_addition_zeros);

    suite_add_tcase(s, tc_neg);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_dm_det_usual)
    {
        double a_arr[] = { 2, 3, 5, 8 };
        doubles_matrix_t a = { 0 };
        assert_success(dm_from_array(&a, a_arr, 2, 2));
        double det;
        assert_success(dm_det(&a, &det));
        assert_double_equals(det, 1.0);

        dm_free(&a);
    }
END_TEST

START_TEST(test_dm_det_zeros)
    {
        double a_arr[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        doubles_matrix_t a = { 0 };
        assert_success(dm_from_array(&a, a_arr, 3, 3));
        double det;
        assert_success(dm_det(&a, &det));
        assert_double_equals(det, 0.0);

        dm_free(&a);
    }
END_TEST

START_TEST(test_dm_det_invalid_size)
    {
        double a_arr[] = { 2, 3, 5, 8, 5, 6 };
        doubles_matrix_t a = { 0 };
        assert_success(dm_from_array(&a, a_arr, 2, 3));
        double det;
        assert_failure(dm_det(&a, &det));

        dm_free(&a);
    }
END_TEST

START_TEST(test_dm_det_not_initialized)
    {
        doubles_matrix_t a = { 0 };
        double det;
        assert_failure(dm_det(&a, &det));
    }
END_TEST

START_TEST(test_dm_det_null_pointers)
    {
        assert_failure(dm_det(NULL, NULL));
    }
END_TEST

Suite *dm_det_suite(Suite *s)
{
    TCase *tc_neg, *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_dm_det_invalid_size);
    tcase_add_test(tc_neg, test_dm_det_not_initialized);
    tcase_add_test(tc_neg, test_dm_det_null_pointers);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_dm_det_usual);
    tcase_add_test(tc_pos, test_dm_det_zeros);

    suite_add_tcase(s, tc_neg);
    suite_add_tcase(s, tc_pos);

    return s;
}
