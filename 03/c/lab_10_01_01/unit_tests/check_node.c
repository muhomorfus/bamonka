#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "node.h"

int compare_ints(const void *a, const void *b)
{
    const int *ai = a;
    const int *bi = b;
    return *ai - *bi;
}

node_t *list_from_arr(int *arr, size_t len)
{
    node_t *head = calloc(1, sizeof(node_t));
    ck_assert_ptr_ne(head, NULL);

    head->data = arr;
    node_t *last = head;
    for (size_t i = 1; i < len; i++)
    {
        last->next = calloc(1, sizeof(node_t));
        ck_assert_ptr_ne(last->next, NULL);
        last->next->data = arr + i;
        last = last->next;
    }

    return head;
}

void cmp_list_and_arr(node_t *head, int *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        ck_assert_ptr_ne(head, NULL);
        ck_assert_int_eq(compare_ints(head->data, arr + i), 0);
        head = head->next;
    }
    ck_assert_ptr_eq(head, NULL);
}

START_TEST(test_pop_front_usual)
{
    int arr[] = { 1, 2, 3, 4 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 2, 3, 4 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    int *res = pop_front(&list);
    ck_assert_int_eq(*res, 1);
    cmp_list_and_arr(list, result, result_len);
    nodes_free(&list);
}
END_TEST

START_TEST(test_pop_front_one_element)
{
    int arr[] = { 8 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    int *res = pop_front(&list);
    ck_assert_int_eq(*res, 8);
    ck_assert_ptr_eq(list, NULL);
    nodes_free(&list);
}
END_TEST

START_TEST(test_pop_front_null_pointer)
{
    void *res = pop_front(NULL);
    ck_assert_ptr_eq(res, NULL);
}
END_TEST

Suite* pop_front_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_pop_front_null_pointer);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_pop_front_usual);
    tcase_add_test(tc_pos, test_pop_front_one_element);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_pop_back_usual)
{
    int arr[] = { 1, 2, 3, 4 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 1, 2, 3 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    int *res = pop_back(&list);
    ck_assert_int_eq(*res, 4);
    cmp_list_and_arr(list, result, result_len);
    nodes_free(&list);
}
END_TEST

START_TEST(test_pop_back_one_element)
{
    int arr[] = { 8 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    int *res = pop_back(&list);
    ck_assert_int_eq(*res, 8);
    ck_assert_ptr_eq(list, NULL);
    nodes_free(&list);
}
END_TEST

START_TEST(test_pop_back_null_pointer)
{
    void *res = pop_back(NULL);
    ck_assert_ptr_eq(res, NULL);
}
END_TEST

Suite* pop_back_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_pop_back_null_pointer);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_pop_back_usual);
    tcase_add_test(tc_pos, test_pop_back_one_element);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_remove_duplicates_no_duplicates)
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 1, 2, 3, 4, 5, 6, 7 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    remove_duplicates(&list, compare_ints);
    cmp_list_and_arr(list, result, result_len);
    nodes_free(&list);
}
END_TEST

START_TEST(test_remove_duplicates_all_duplicates)
{
    int arr[] = { 1, 1, 1, 1, 1 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 1 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    remove_duplicates(&list, compare_ints);
    cmp_list_and_arr(list, result, result_len);
    nodes_free(&list);
}
END_TEST

START_TEST(test_remove_duplicates_usual)
{
    int arr[] = { 1, 2, 2, 3, 4, 5, 5, 5, 6, 8, 9, 9 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 1, 2, 3, 4, 5, 6, 8, 9 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    remove_duplicates(&list, compare_ints);
    cmp_list_and_arr(list, result, result_len);
    nodes_free(&list);
}
END_TEST

Suite* remove_duplicates_suite(Suite *s)
{
    TCase *tc_pos;

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_remove_duplicates_no_duplicates);
    tcase_add_test(tc_pos, test_remove_duplicates_all_duplicates);
    tcase_add_test(tc_pos, test_remove_duplicates_usual);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_sorted_insert_to_empty)
{
//        int arr[] = { 1, 2, 3, 4, 5, 7 };
//        size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 6 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = NULL;
//        ck_assert_ptr_ne(list, NULL);

    node_t *node = calloc(1, sizeof(node_t));
    ck_assert_ptr_ne(node, NULL);
    int val = 6;
    node->data = &val;

    sorted_insert(&list, node, compare_ints);
    cmp_list_and_arr(list, result, result_len);
    nodes_free(&list);
}
END_TEST

START_TEST(test_sorted_insert_to_equals)
{
    int arr[] = { 1, 2, 3, 4, 4, 4, 5, 7 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 1, 2, 3, 4, 4, 4, 4, 5, 7 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    node_t *node = calloc(1, sizeof(node_t));
    ck_assert_ptr_ne(node, NULL);
    int val = 4;
    node->data = &val;

    sorted_insert(&list, node, compare_ints);
    cmp_list_and_arr(list, result, result_len);
    nodes_free(&list);
}
END_TEST

START_TEST(test_sorted_insert_usual)
{
    int arr[] = { 1, 2, 3, 4, 5, 7 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 1, 2, 3, 4, 5, 6, 7 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    node_t *node = calloc(1, sizeof(node_t));
    ck_assert_ptr_ne(node, NULL);
    int val = 6;
    node->data = &val;

    sorted_insert(&list, node, compare_ints);
    cmp_list_and_arr(list, result, result_len);
    nodes_free(&list);
}
END_TEST

START_TEST(test_sorted_insert_to_end)
{
    int arr[] = { 1, 2, 3, 4, 5, 7 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 1, 2, 3, 4, 5, 7, 8 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    node_t *node = calloc(1, sizeof(node_t));
    ck_assert_ptr_ne(node, NULL);
    int val = 8;
    node->data = &val;

    sorted_insert(&list, node, compare_ints);
    cmp_list_and_arr(list, result, result_len);
    nodes_free(&list);
}
END_TEST

Suite* sorted_insert_suite(Suite *s)
{
    TCase *tc_pos;

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_sorted_insert_to_empty);
    tcase_add_test(tc_pos, test_sorted_insert_to_equals);
    tcase_add_test(tc_pos, test_sorted_insert_usual);
    tcase_add_test(tc_pos, test_sorted_insert_to_end);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_sort_usual)
{
    int arr[] = { 4, 5, 2, 3, 1, 1, 9, 4, 3 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 1, 1, 2, 3, 3, 4, 4, 5, 9 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    node_t *sorted = sort(list, compare_ints);
    ck_assert_ptr_ne(sorted, NULL);
    cmp_list_and_arr(sorted, result, result_len);
    nodes_free(&list);
    nodes_free(&sorted);
}
END_TEST

START_TEST(test_sort_sorted)
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    node_t *sorted = sort(list, compare_ints);
    ck_assert_ptr_ne(sorted, NULL);
    cmp_list_and_arr(sorted, result, result_len);
    nodes_free(&list);
    nodes_free(&sorted);
}
END_TEST

START_TEST(test_sort_reversed)
{
    int arr[] = { 8, 7, 6, 5, 4, 3, 2, 1 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    node_t *sorted = sort(list, compare_ints);
    ck_assert_ptr_ne(sorted, NULL);
    cmp_list_and_arr(sorted, result, result_len);
    nodes_free(&list);
    nodes_free(&sorted);
}
END_TEST

START_TEST(test_sort_one_element)
{
    int arr[] = { 4 };
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);

    int result[] = { 4 };
    size_t result_len = sizeof(result) / sizeof(result[0]);

    node_t *list = list_from_arr(arr, arr_len);
    ck_assert_ptr_ne(list, NULL);

    node_t *sorted = sort(list, compare_ints);
    ck_assert_ptr_ne(sorted, NULL);
    cmp_list_and_arr(sorted, result, result_len);
    nodes_free(&list);
    nodes_free(&sorted);
}
END_TEST

Suite* sort_suite(Suite *s)
{
    TCase *tc_pos;

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_sort_usual);
    tcase_add_test(tc_pos, test_sort_sorted);
    tcase_add_test(tc_pos, test_sort_reversed);
    tcase_add_test(tc_pos, test_sort_one_element);
    suite_add_tcase(s, tc_pos);

    return s;
}