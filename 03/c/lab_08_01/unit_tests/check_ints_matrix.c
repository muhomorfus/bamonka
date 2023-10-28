#include <stdlib.h>
#include <check.h>

#include "ints_matrix.h"
#include "error.h"

//error_t im_increase_to_size(ints_matrix_t *m, size_t rows, size_t cols);

void assert_success(error_t err)
{
    ck_assert_int_eq(is_failure(err), 0);
}

void assert_failure(error_t err)
{
    ck_assert_int_ne(is_failure(err), 0);
}

void im_assert_equals(ints_matrix_t *a, ints_matrix_t *b)
{
    ck_assert_int_eq(a->rows, b->rows);
    ck_assert_int_eq(a->cols, b->cols);

    for (size_t i = 0; i < a->rows; i++)
        for (size_t j = 0; j < a->cols; j++)
            ck_assert_int_eq(a->ptr[i][j], b->ptr[i][j]);
}

error_t im_from_array(ints_matrix_t *m, int *arr, size_t rows, size_t cols)
{
    error_t err;
    if (is_failure(err = im_init(m, rows, cols)))
        return err;

    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            m->ptr[i][j] = arr[i * cols + j];

    return NEW_SUCCESS();
}

START_TEST(test_im_fill_identity_usual)
{
    ints_matrix_t a = { 0 };
    assert_success(im_init(&a, 3, 3));
    assert_success(im_fill_identity(&a));
    int arr[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    ints_matrix_t res = { 0 };
    assert_success(im_from_array(&res, arr, 3, 3));
    im_assert_equals(&a, &res);
    im_free(&a);
    im_free(&res);
}
END_TEST

START_TEST(test_im_fill_identity_non_equal_sizes)
{
    ints_matrix_t a = { 0 };
    assert_success(im_init(&a, 3, 4));
    assert_failure(im_fill_identity(&a));
    im_free(&a);
}
END_TEST

START_TEST(test_im_fill_identity_not_initialized)
{
    ints_matrix_t a = { 0 };
    assert_failure(im_fill_identity(&a));
}
END_TEST

START_TEST(test_im_fill_identity_null_pointer)
{
    assert_failure(im_fill_identity(NULL));
}
END_TEST

Suite* im_fill_identity_suite(Suite *s)
{
    TCase *tc_neg, *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_im_fill_identity_non_equal_sizes);
    tcase_add_test(tc_neg, test_im_fill_identity_not_initialized);
    tcase_add_test(tc_neg, test_im_fill_identity_null_pointer);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_im_fill_identity_usual);

    suite_add_tcase(s, tc_neg);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_im_fill_multiply_usual)
{
    int a_arr[] = { 1, 2, 3, 4, 5, 6 };
    int b_arr[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
    ints_matrix_t a = { 0 }, b = { 0 }, c = { 0 };
    assert_success(im_from_array(&a, a_arr, 3, 2));
    assert_success(im_from_array(&b, b_arr, 2, 4));
    assert_success(im_init(&c, 3, 4));
    assert_success(im_fill_multiply(&c, &a, &b));

    int arr[] = { 13, 10, 7, 4, 33, 26, 19, 12, 53, 42, 31, 20 };
    ints_matrix_t res = { 0 };
    assert_success(im_from_array(&res, arr, 3, 4));
    im_assert_equals(&c, &res);
    im_free(&a);
    im_free(&b);
    im_free(&c);
    im_free(&res);
}
END_TEST

START_TEST(test_im_fill_multiply_identity)
{
    int a_arr[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    int b_arr[] = { 7, 6, 5, 4, 3, 2 };
    ints_matrix_t a = { 0 }, b = { 0 }, c = { 0 };
    assert_success(im_from_array(&a, a_arr, 3, 3));
    assert_success(im_from_array(&b, b_arr, 3, 2));
    assert_success(im_init(&c, 3, 2));
    assert_success(im_fill_multiply(&c, &a, &b));

    int arr[] = { 7, 6, 5, 4, 3, 2 };
    ints_matrix_t res = { 0 };
    assert_success(im_from_array(&res, arr, 3, 2));
    im_assert_equals(&c, &res);
    im_free(&a);
    im_free(&b);
    im_free(&c);
    im_free(&res);
}
END_TEST

START_TEST(test_im_fill_multiply_invalid_sizes)
{
    int a_arr[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    int b_arr[] = { 7, 6, 5, 4 };
    ints_matrix_t a = { 0 }, b = { 0 }, c = { 0 };
    assert_success(im_from_array(&a, a_arr, 3, 3));
    assert_success(im_from_array(&b, b_arr, 2, 2));
    assert_success(im_init(&c, 3, 4));
    assert_failure(im_fill_multiply(&c, &a, &b));

    im_free(&a);
    im_free(&b);
    im_free(&c);
}
END_TEST

START_TEST(test_im_fill_multiply_not_initialized)
{
    ints_matrix_t a = { 0 }, b = { 0 }, c = { 0 };
    assert_failure(im_fill_multiply(&c, &a, &b));
}
END_TEST

START_TEST(test_im_fill_multiply_null_pointers)
{
    assert_failure(im_fill_multiply(NULL, NULL, NULL));
}
END_TEST

Suite* im_fill_multiply_suite(Suite *s)
{
    TCase *tc_neg, *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_im_fill_multiply_invalid_sizes);
    tcase_add_test(tc_neg, test_im_fill_multiply_not_initialized);
    tcase_add_test(tc_neg, test_im_fill_multiply_null_pointers);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_im_fill_multiply_usual);
    tcase_add_test(tc_pos, test_im_fill_multiply_identity);

    suite_add_tcase(s, tc_neg);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_im_fill_by_matrix_usual)
{
    int b_arr[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
    ints_matrix_t a = { 0 }, b = { 0 };
    assert_success(im_init(&a, 2, 2));
    assert_success(im_from_array(&b, b_arr, 2, 4));
    assert_success(im_fill_by_matrix(&a, &b));

    int arr[] = { 7, 6, 3, 2 };
    ints_matrix_t res = { 0 };
    assert_success(im_from_array(&res, arr, 2, 2));
    im_assert_equals(&a, &res);
    im_free(&a);
    im_free(&b);
    im_free(&res);
}
END_TEST

START_TEST(test_im_fill_by_matrix_eq_sizes)
{
    int b_arr[] = { 7, 6, 5, 4 };
    ints_matrix_t a = { 0 }, b = { 0 };
    assert_success(im_init(&a, 2, 2));
    assert_success(im_from_array(&b, b_arr, 2, 2));
    assert_success(im_fill_by_matrix(&a, &b));

    int arr[] = { 7, 6, 5, 4 };
    ints_matrix_t res = { 0 };
    assert_success(im_from_array(&res, arr, 2, 2));
    im_assert_equals(&a, &res);
    im_free(&a);
    im_free(&b);
    im_free(&res);
}
END_TEST

START_TEST(test_im_fill_by_matrix_invalid_num_of_cols)
{
    int b_arr[] = { 7, 6, 5, 4 };
    ints_matrix_t a = { 0 }, b = { 0 };
    assert_success(im_init(&a, 2, 2));
    assert_success(im_from_array(&b, b_arr, 2, 1));
    assert_failure(im_fill_by_matrix(&a, &b));

    im_free(&a);
    im_free(&b);
}
END_TEST

START_TEST(test_im_fill_by_matrix_invalid_num_of_rows)
{
    int b_arr[] = { 7, 6, 5, 4 };
    ints_matrix_t a = { 0 }, b = { 0 };
    assert_success(im_init(&a, 2, 2));
    assert_success(im_from_array(&b, b_arr, 1, 2));
    assert_failure(im_fill_by_matrix(&a, &b));

    im_free(&a);
    im_free(&b);
}
END_TEST

START_TEST(test_im_fill_by_matrix_not_initialized)
{
    ints_matrix_t a = { 0 }, b = { 0 };
    assert_failure(im_fill_by_matrix(&a, &b));
}
END_TEST

START_TEST(test_im_fill_by_matrix_null_pointers)
{
    assert_failure(im_fill_by_matrix(NULL, NULL));
}
END_TEST

Suite* im_fill_by_matrix_suite(Suite *s)
{
    TCase *tc_neg, *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_im_fill_by_matrix_invalid_num_of_cols);
    tcase_add_test(tc_neg, test_im_fill_by_matrix_invalid_num_of_rows);
    tcase_add_test(tc_neg, test_im_fill_by_matrix_not_initialized);
    tcase_add_test(tc_neg, test_im_fill_by_matrix_null_pointers);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_im_fill_by_matrix_usual);
    tcase_add_test(tc_pos, test_im_fill_by_matrix_eq_sizes);

    suite_add_tcase(s, tc_neg);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_im_delete_col_usual)
{
    int a_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ints_matrix_t a = { 0 };
    assert_success(im_from_array(&a, a_arr, 3, 3));
    assert_success(im_delete_col(&a, 1));
    
    int arr[] = { 1, 3, 4, 6, 7, 9 };
    ints_matrix_t res = { 0 };
    assert_success(im_from_array(&res, arr, 3, 2));

    im_assert_equals(&a, &res);
    
    im_free(&a);
    im_free(&res);
}
END_TEST

START_TEST(test_im_delete_col_invalid_column)
{
    int a_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ints_matrix_t a = { 0 };
    assert_success(im_from_array(&a, a_arr, 3, 3));
    assert_failure(im_delete_col(&a, 4));

    im_free(&a);
}
END_TEST

START_TEST(test_im_delete_col_not_initialized)
{
    ints_matrix_t a = { 0 };
    assert_failure(im_delete_col(&a, 1));
}
END_TEST

START_TEST(test_im_delete_col_null_pointers)
{
    assert_failure(im_delete_col(NULL, 1));
}
END_TEST

Suite* im_delete_col_suite(Suite *s)
{
    TCase *tc_neg, *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_im_delete_col_invalid_column);
    tcase_add_test(tc_neg, test_im_delete_col_not_initialized);
    tcase_add_test(tc_neg, test_im_delete_col_null_pointers);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_im_delete_col_usual);

    suite_add_tcase(s, tc_neg);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_im_delete_row_usual)
{
    int a_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ints_matrix_t a = { 0 };
    assert_success(im_from_array(&a, a_arr, 3, 3));
    assert_success(im_delete_row(&a, 1));
    
    int arr[] = { 1, 2, 3, 7, 8, 9 };
    ints_matrix_t res = { 0 };
    assert_success(im_from_array(&res, arr, 2, 3));

    im_assert_equals(&a, &res);
    
    im_free(&a);
    im_free(&res);
}
END_TEST

START_TEST(test_im_delete_row_invalid_row)
{
    int a_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ints_matrix_t a = { 0 };
    assert_success(im_from_array(&a, a_arr, 3, 3));
    assert_failure(im_delete_row(&a, 4));

    im_free(&a);
}
END_TEST

START_TEST(test_im_delete_row_not_initialized)
{
    ints_matrix_t a = { 0 };
    assert_failure(im_delete_row(&a, 1));
}
END_TEST

START_TEST(test_im_delete_row_null_pointers)
{
    assert_failure(im_delete_row(NULL, 1));
}
END_TEST

Suite* im_delete_row_suite(Suite *s)
{
    TCase *tc_neg, *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_im_delete_row_invalid_row);
    tcase_add_test(tc_neg, test_im_delete_row_not_initialized);
    tcase_add_test(tc_neg, test_im_delete_row_null_pointers);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_im_delete_row_usual);

    suite_add_tcase(s, tc_neg);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_im_max_element_pos_usual)
{
    int a_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ints_matrix_t a = { 0 };
    assert_success(im_from_array(&a, a_arr, 3, 3));

    size_t i, j;
    assert_success(im_max_element_pos(&a, &i, &j));
    ck_assert_int_eq(i, 2);
    ck_assert_int_eq(j, 2);
    im_free(&a);
}
END_TEST

START_TEST(test_im_max_element_pos_many_maxes)
{
    int a_arr[] = { 1, 2, 2, 1, 2, 1, 0, 0, 2 };
    ints_matrix_t a = { 0 };
    assert_success(im_from_array(&a, a_arr, 3, 3));

    size_t i, j;
    assert_success(im_max_element_pos(&a, &i, &j));
    ck_assert_int_eq(i, 0);
    ck_assert_int_eq(j, 1);
    im_free(&a);
}
END_TEST

START_TEST(test_im_max_element_pos_not_initialized)
{
    ints_matrix_t a = { 0 };

    size_t i, j;
    assert_failure(im_max_element_pos(&a, &i, &j));
}
END_TEST

START_TEST(test_im_max_element_pos_null_pointers)
{
    assert_failure(im_max_element_pos(NULL, NULL, NULL));
}
END_TEST

Suite* im_max_element_pos_suite(Suite *s)
{
    TCase *tc_neg, *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_im_max_element_pos_not_initialized);
    tcase_add_test(tc_neg, test_im_max_element_pos_null_pointers);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_im_max_element_pos_usual);
    tcase_add_test(tc_pos, test_im_max_element_pos_many_maxes);

    suite_add_tcase(s, tc_neg);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_im_make_square_usual)
{
    int a_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    ints_matrix_t a = { 0 };
    assert_success(im_from_array(&a, a_arr, 2, 5));
    assert_success(im_make_square(&a));


    int arr[] = { 1, 5, 6, 0 };
    ints_matrix_t res = { 0 };
    assert_success(im_from_array(&res, arr, 2, 2));
    im_assert_equals(&a, &res);

    im_free(&a);
    im_free(&res);
}
END_TEST

START_TEST(test_im_make_square_already_squared)
{
    int a_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ints_matrix_t a = { 0 };
    assert_success(im_from_array(&a, a_arr, 3, 3));
    assert_success(im_make_square(&a));


    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ints_matrix_t res = { 0 };
    assert_success(im_from_array(&res, arr, 3, 3));
    im_assert_equals(&a, &res);

    im_free(&a);
    im_free(&res);
}
END_TEST

START_TEST(test_im_make_square_not_initialized)
{
    ints_matrix_t a = { 0 };
    assert_failure(im_make_square(&a));
}
END_TEST

START_TEST(test_im_make_square_null_pointer)
{
    assert_failure(im_make_square(NULL));
}
END_TEST

Suite* im_make_square_suite(Suite *s)
{
    TCase *tc_neg, *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_im_make_square_not_initialized);
    tcase_add_test(tc_neg, test_im_make_square_null_pointer);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_im_make_square_usual);
    tcase_add_test(tc_pos, test_im_make_square_already_squared);

    suite_add_tcase(s, tc_neg);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_im_increase_to_size_usual)
{
    int a_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    ints_matrix_t a = { 0 };
    assert_success(im_from_array(&a, a_arr, 2, 5));
    assert_success(im_increase_to_size(&a, 3, 6));


    int arr[] = { 1, 2, 3, 4, 5, 5, 6, 7, 8, 9, 0, 9, 3, 4, 5, 6, 2, 6 };
    ints_matrix_t res = { 0 };
    assert_success(im_from_array(&res, arr, 3, 6));
    im_assert_equals(&a, &res);

    im_free(&a);
    im_free(&res);
}
END_TEST

START_TEST(test_im_increase_to_size_already_sized)
{
    int a_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    ints_matrix_t a = { 0 };
    assert_success(im_from_array(&a, a_arr, 2, 5));
    assert_success(im_increase_to_size(&a, 2, 5));


    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    ints_matrix_t res = { 0 };
    assert_success(im_from_array(&res, arr, 2, 5));
    im_assert_equals(&a, &res);

    im_free(&a);
    im_free(&res);
}
END_TEST

START_TEST(test_im_increase_to_size_size_is_less)
{
    int a_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    ints_matrix_t a = { 0 };
    assert_success(im_from_array(&a, a_arr, 2, 5));
    assert_failure(im_increase_to_size(&a, 2, 4));

    im_free(&a);
}
END_TEST

START_TEST(test_im_increase_to_size_not_initialized)
{
    ints_matrix_t a = { 0 };
    assert_failure(im_increase_to_size(&a, 2, 4));
}
END_TEST

START_TEST(test_im_increase_to_size_null_pointer)
{
    assert_failure(im_increase_to_size(NULL, 2, 4));
}
END_TEST

Suite* im_increase_to_size_suite(Suite *s)
{
    TCase *tc_neg, *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_im_increase_to_size_size_is_less);
    tcase_add_test(tc_neg, test_im_increase_to_size_not_initialized);
    tcase_add_test(tc_neg, test_im_increase_to_size_null_pointer);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_im_increase_to_size_usual);
    tcase_add_test(tc_pos, test_im_increase_to_size_already_sized);

    suite_add_tcase(s, tc_neg);
    suite_add_tcase(s, tc_pos);

    return s;
}

