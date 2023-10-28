#include <stdlib.h>
#include <check.h>

#include "array.h"

START_TEST(test_new_ints_array_null_pointer)
{
    int rc = new_ints_array(1, NULL);
    ck_assert_int_eq(rc, ERROR_NULL_POINTER);
}
END_TEST

START_TEST(test_new_ints_array_zero_size)
{
    int *arr = NULL;
    int rc = new_ints_array(0, &arr);
    ck_assert_int_eq(rc, ERROR_EMPTY);
}
END_TEST

Suite* new_ints_array_suite(Suite *s)
{
    TCase *tc_neg;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_new_ints_array_null_pointer);
    tcase_add_test(tc_neg, test_new_ints_array_zero_size);
    suite_add_tcase(s, tc_neg);

    return s;
}

START_TEST(test_key_null_pb_src)
{
    int arr[] = {1, 2, 3, 4, 5};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    int *pb, *pe;
    int rc = key(NULL, arr + n, &pb, &pe);
    ck_assert_int_eq(rc, ERROR_NULL_POINTER);
}
END_TEST

START_TEST(test_key_null_pe_src)
{
    int arr[] = {1, 2, 3, 4, 5};
    int *pb, *pe;
    int rc = key(arr, NULL, &pb, &pe);
    ck_assert_int_eq(rc, ERROR_NULL_POINTER);
}
END_TEST

START_TEST(test_key_null_pb_dst)
{
    int arr[] = {1, 2, 3, 4, 5};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    int *pe;
    int rc = key(arr, arr + n, NULL, &pe);
    ck_assert_int_eq(rc, ERROR_NULL_POINTER);
}
END_TEST

START_TEST(test_key_null_pe_dst)
{
    int arr[] = {1, 2, 3, 4, 5};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    int *pb;
    int rc = key(arr, arr + n, &pb, NULL);
    ck_assert_int_eq(rc, ERROR_NULL_POINTER);
}
END_TEST

START_TEST(test_key_empty_array)
{
    int arr[] = {1, 2, 3, 4, 5};
    int *pb, *pe;
    int rc = key(arr, arr, &pb, &pe);
    ck_assert_int_eq(rc, ERROR_EMPTY);
}
END_TEST

START_TEST(test_key_overlapping_pointers)
{
    int arr[] = {1, 2, 3, 4, 5};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    int *pb, *pe;
    int rc = key(arr + n, arr, &pb, &pe);
    ck_assert_int_eq(rc, ERROR_OVERLAP_POINTERS);
}
END_TEST

START_TEST(test_key_empty_filtering)
{
    int arr[] = {1, 1, 1};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    int *pb, *pe;
    int rc = key(arr, arr + n, &pb, &pe);
    ck_assert_int_eq(rc, ERROR_EMPTY_FILTER);
}
END_TEST

START_TEST(test_key_normal_filtering)
{
    int arr[] = {1, 2, 6, 3, 8, 2, 3};
    int res[] = {6, 8};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    size_t res_n = sizeof(res) / sizeof(res[0]);
    int *pb, *pe;
    int rc = key(arr, arr + n, &pb, &pe);
    ck_assert_int_eq(rc, EXIT_SUCCESS);
    ck_assert_int_eq(res_n, pe - pb);
    for (size_t i = 0; i < res_n; i++)
        ck_assert_int_eq(res[i], pb[i]);
    free(pb);
}
END_TEST

Suite* key_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_key_null_pb_src);
    tcase_add_test(tc_neg, test_key_null_pe_src);
    tcase_add_test(tc_neg, test_key_null_pb_dst);
    tcase_add_test(tc_neg, test_key_null_pe_dst);
    tcase_add_test(tc_neg, test_key_empty_array);
    tcase_add_test(tc_neg, test_key_overlapping_pointers);
    tcase_add_test(tc_neg, test_key_empty_filtering);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_key_normal_filtering);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_compare_ints_first_less)
{
    int a = 1;
    int b = 10;
    int c = compare_ints((void *) (&a), (void *) (&b));
    ck_assert_int_lt(c, 0);
}
END_TEST

START_TEST(test_compare_ints_equals)
{
    int a = 1;
    int b = 1;
    int c = compare_ints((void *) (&a), (void *) (&b));
    ck_assert_int_eq(c, 0);
}
END_TEST

START_TEST(test_compare_ints_second_less)
{
    int a = 10;
    int b = 1;
    int c = compare_ints((void *) (&a), (void *) (&b));
    ck_assert_int_gt(c, 0);
}
END_TEST

Suite* compare_ints_suite(Suite *s)
{
    TCase *tc_pos;

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_compare_ints_first_less);
    tcase_add_test(tc_pos, test_compare_ints_equals);
    tcase_add_test(tc_pos, test_compare_ints_second_less);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_mysort_sorted)
{
    int arr[] = {1, 2, 3, 4, 5, 6};
    size_t len = sizeof(arr) / sizeof(arr[0]);

    int res[] = {1, 2, 3, 4, 5, 6};
    mysort(arr, len, sizeof(arr[0]), compare_ints);
    for (size_t i = 0; i < len; i++)
        ck_assert_int_eq(arr[i], res[i]);
}
END_TEST

START_TEST(test_mysort_reversed)
{
    int arr[] = {6, 5, 4, 3, 2, 1};
    size_t len = sizeof(arr) / sizeof(arr[0]);

    int res[] = {1, 2, 3, 4, 5, 6};
    mysort(arr, len, sizeof(arr[0]), compare_ints);
    for (size_t i = 0; i < len; i++)
        ck_assert_int_eq(arr[i], res[i]);
}
END_TEST

START_TEST(test_mysort_mixed)
{
    int arr[] = {4, 2, 3, 6, 5, 1};
    size_t len = sizeof(arr) / sizeof(arr[0]);

    int res[] = {1, 2, 3, 4, 5, 6};
    mysort(arr, len, sizeof(arr[0]), compare_ints);
    for (size_t i = 0; i < len; i++)
        ck_assert_int_eq(arr[i], res[i]);
}
END_TEST

START_TEST(test_mysort_have_duplicates)
{
    int arr[] = {4, 3, 6, 5, 5, 4, 1};
    size_t len = sizeof(arr) / sizeof(arr[0]);

    int res[] = {1, 3, 4, 4, 5, 5, 6};
    mysort(arr, len, sizeof(arr[0]), compare_ints);
    for (size_t i = 0; i < len; i++)
        ck_assert_int_eq(arr[i], res[i]);
}
END_TEST

START_TEST(test_mysort_one_element)
{
    int arr[] = {1};
    size_t len = sizeof(arr) / sizeof(arr[0]);

    int res[] = {1};
    mysort(arr, len, sizeof(arr[0]), compare_ints);
    for (size_t i = 0; i < len; i++)
        ck_assert_int_eq(arr[i], res[i]);
}
END_TEST

START_TEST(test_mysort_all_equals)
{
    int arr[] = {4, 4, 4, 4, 4, 4, 4};
    size_t len = sizeof(arr) / sizeof(arr[0]);

    int res[] = {4, 4, 4, 4, 4, 4, 4};
    mysort(arr, len, sizeof(arr[0]), compare_ints);
    for (size_t i = 0; i < len; i++)
        ck_assert_int_eq(arr[i], res[i]);
}
END_TEST

START_TEST(test_mysort_base_is_null)
{
    int arr[] = {1, 4, 3};
    size_t len = sizeof(arr) / sizeof(arr[0]);

    int res[] = {1, 4, 3};
    mysort(NULL, len, sizeof(arr[0]), compare_ints);
    for (size_t i = 0; i < len; i++)
        ck_assert_int_eq(arr[i], res[i]);
}
END_TEST

START_TEST(test_mysort_nmemb_is_zero)
{
    int arr[] = {1, 4, 3};
    size_t len = sizeof(arr) / sizeof(arr[0]);
    int res[] = {1, 4, 3};
    mysort(NULL, 0, sizeof(arr[0]), compare_ints);
    for (size_t i = 0; i < len; i++)
        ck_assert_int_eq(arr[i], res[i]);
}
END_TEST

START_TEST(test_mysort_size_is_zero)
{
    int arr[] = {1, 4, 3};
    size_t len = sizeof(arr) / sizeof(arr[0]);

    int res[] = {1, 4, 3};
    mysort(NULL, len, 0, compare_ints);
    for (size_t i = 0; i < len; i++)
        ck_assert_int_eq(arr[i], res[i]);
}
END_TEST

START_TEST(test_mysort_compar_is_null)
{
    int arr[] = {1, 4, 3};
    size_t len = sizeof(arr) / sizeof(arr[0]);

    int res[] = {1, 4, 3};
    mysort(NULL, len, sizeof(arr[0]), NULL);
    for (size_t i = 0; i < len; i++)
        ck_assert_int_eq(arr[i], res[i]);
}
END_TEST

Suite* mysort_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_mysort_base_is_null);
    tcase_add_test(tc_neg, test_mysort_nmemb_is_zero);
    tcase_add_test(tc_neg, test_mysort_size_is_zero);
    tcase_add_test(tc_neg, test_mysort_compar_is_null);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_mysort_sorted);
    tcase_add_test(tc_pos, test_mysort_reversed);
    tcase_add_test(tc_pos, test_mysort_mixed);
    tcase_add_test(tc_pos, test_mysort_have_duplicates);
    tcase_add_test(tc_pos, test_mysort_one_element);
    tcase_add_test(tc_pos, test_mysort_all_equals);
    suite_add_tcase(s, tc_pos);

    return s;
}