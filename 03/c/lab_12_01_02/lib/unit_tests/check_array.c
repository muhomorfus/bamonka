#include <stdlib.h>
#include <check.h>

#include "array.h"

void cmp_arrays(int *arr1, size_t size1, int *arr2, size_t size2)
{
    ck_assert_int_eq(size1, size2);
    for (size_t i = 0; i < size1; i++)
        ck_assert_int_eq(arr1[i], arr2[i]);
}

START_TEST(test_arr_lshift_one_element)
{
    int arr[] = { 1 };
    size_t arr_size = sizeof(arr) / sizeof(arr[0]);
    int err = arr_lshift(arr, arr_size, 3);
    ck_assert_int_eq(err, EXIT_SUCCESS);

    int result[] = { 1 };
    size_t result_size = sizeof(result) / sizeof(result[0]);

    cmp_arrays(arr, arr_size, result, result_size);
}
END_TEST

START_TEST(test_arr_lshift_usual)
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7 };
    size_t arr_size = sizeof(arr) / sizeof(arr[0]);
    int err = arr_lshift(arr, arr_size, 3);
    ck_assert_int_eq(err, EXIT_SUCCESS);

    int result[] = { 4, 5, 6, 7, 1, 2, 3 };
    size_t result_size = sizeof(result) / sizeof(result[0]);

    cmp_arrays(arr, arr_size, result, result_size);
}
END_TEST

START_TEST(test_arr_lshift_shift_negative)
{
    int arr[] = { 1, 2, 3, 4, 5 };
    size_t arr_size = sizeof(arr) / sizeof(arr[0]);
    int err = arr_lshift(arr, arr_size, -2);
    ck_assert_int_eq(err, EXIT_SUCCESS);

    int result[] = { 4, 5, 1, 2, 3 };
    size_t result_size = sizeof(result) / sizeof(result[0]);

    cmp_arrays(arr, arr_size, result, result_size);
}
END_TEST

START_TEST(test_arr_lshift_null_pointer)
{
    int err = arr_lshift(NULL, 1, 3);
    ck_assert_int_ne(err, EXIT_SUCCESS);
}
END_TEST

Suite* arr_lshift_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_arr_lshift_null_pointer);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_arr_lshift_one_element);
    tcase_add_test(tc_pos, test_arr_lshift_usual);
    tcase_add_test(tc_pos, test_arr_lshift_shift_negative);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_arr_copy_squares_no_squares)
{
    int src[] = { 2, 3, 5, 7 };
    size_t src_size = sizeof(src) / sizeof(src[0]);
    int dst[100];
    size_t dst_size;
    int err = arr_copy_squares(src, src_size, dst, &dst_size);
    ck_assert_int_eq(err, EXIT_SUCCESS);
    ck_assert_int_eq(dst_size, 0);
}
END_TEST

START_TEST(test_arr_copy_squares_usual)
{
    int src[] = { 2, 3, 1, 3, 6, 9, 6, 16 };
    size_t src_size = sizeof(src) / sizeof(src[0]);
    int dst[100];
    size_t dst_size;
    int err = arr_copy_squares(src, src_size, dst, &dst_size);
    ck_assert_int_eq(err, EXIT_SUCCESS);

    int result[] = { 1, 9, 16 };
    size_t result_size = sizeof(result) / sizeof(result[0]);

    cmp_arrays(dst, dst_size, result, result_size);
}
END_TEST

START_TEST(test_arr_copy_count)
{
    int src[] = { 2, 3, 1, 3, 6, 9, 6, 16 };
    size_t src_size = sizeof(src) / sizeof(src[0]);
    size_t dst_size;
    int err = arr_copy_squares(src, src_size, NULL, &dst_size);
    ck_assert_int_eq(err, ARR_NOT_ALLOCATED_ERROR);
    ck_assert_int_eq(dst_size, 3);
}
END_TEST

START_TEST(test_arr_copy_null_pointer)
{
    int err = arr_copy_squares(NULL, 0, NULL, NULL);
    ck_assert_int_ne(err, EXIT_SUCCESS);
}
END_TEST

Suite* arr_copy_null_pointer_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_arr_copy_null_pointer);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_arr_copy_squares_no_squares);
    tcase_add_test(tc_pos, test_arr_copy_squares_usual);
    tcase_add_test(tc_pos, test_arr_copy_count);
    suite_add_tcase(s, tc_pos);

    return s;
}

