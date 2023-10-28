#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "mnode.h"

typedef struct node_content_t
{
    size_t i;
    size_t j;
    int n;
} node_content_t;

mnode_t *list_from_arr(node_content_t *arr, size_t len)
{
    mnode_t *head = NULL;
    mnode_t *cur = NULL;

    for (size_t i = 0; i < len; i++)
    {
        ck_assert_ptr_ne(mlist_append(&head, &cur, arr[i].i, arr[i].j, arr[i].n), NULL);
    }

    return head;
}

void cmp_list_and_arr(mnode_t *head, node_content_t *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        ck_assert_ptr_ne(head, NULL);
        ck_assert_int_eq(head->i, arr[i].i);
        ck_assert_int_eq(head->j, arr[i].j);
        ck_assert_int_eq(head->n, arr[i].n);
        head = head->next;
    }
    ck_assert_ptr_eq(head, NULL);
}

START_TEST(test_mlist_mul_usual)
{
    node_content_t a_arr[] = { {0, 0, 1 }, { 0, 1, 2 }, { 1, 1, 3} };
    size_t a_arr_len = sizeof(a_arr) / sizeof(a_arr[0]);
    mnode_t *a = list_from_arr(a_arr, a_arr_len);

    node_content_t b_arr[] = { {0, 0, 3 }, { 0, 1, 4 } };
    size_t b_arr_len = sizeof(b_arr) / sizeof(b_arr[0]);
    mnode_t *b = list_from_arr(b_arr, b_arr_len);

    node_content_t res_arr[] = { {0, 0, 3 }, { 0, 1, 4 } };
    size_t res_arr_len = sizeof(res_arr) / sizeof(res_arr[0]);

    mnode_t *res = mlist_mul(a, b);

    cmp_list_and_arr(res, res_arr, res_arr_len);

    mlist_free(&a);
    mlist_free(&b);
    mlist_free(&res);
}
END_TEST

START_TEST(test_mlist_mul_identity)
{
    node_content_t a_arr[] = { {0, 0, 1 }, { 0, 1, 2 }, { 1, 1, 3} };
    size_t a_arr_len = sizeof(a_arr) / sizeof(a_arr[0]);
    mnode_t *a = list_from_arr(a_arr, a_arr_len);

    node_content_t b_arr[] = { {0, 0, 1 }, { 1, 1, 1 }, { 2, 2, 1 } };
    size_t b_arr_len = sizeof(b_arr) / sizeof(b_arr[0]);
    mnode_t *b = list_from_arr(b_arr, b_arr_len);

    node_content_t res_arr[] = { {0, 0, 1 }, { 0, 1, 2 }, { 1, 1, 3} };
    size_t res_arr_len = sizeof(res_arr) / sizeof(res_arr[0]);

    mnode_t *res = mlist_mul(a, b);

    cmp_list_and_arr(res, res_arr, res_arr_len);

    mlist_free(&a);
    mlist_free(&b);
    mlist_free(&res);
}
END_TEST

START_TEST(test_mlist_mul_null_pointer)
{
    ck_assert_ptr_eq(mlist_mul(NULL, NULL), NULL);
}
END_TEST

Suite* mlist_mul_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_mlist_mul_null_pointer);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_mlist_mul_usual);
    tcase_add_test(tc_pos, test_mlist_mul_identity);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_mlist_add_usual)
{
    node_content_t a_arr[] = { {0, 0, 1 }, { 0, 1, 2 }, { 1, 1, 3}, { 2, 1, 3 } };
    size_t a_arr_len = sizeof(a_arr) / sizeof(a_arr[0]);
    mnode_t *a = list_from_arr(a_arr, a_arr_len);

    node_content_t b_arr[] = { {0, 0, 3 }, { 0, 1, 4 }, { 1, 1, -3 } };
    size_t b_arr_len = sizeof(b_arr) / sizeof(b_arr[0]);
    mnode_t *b = list_from_arr(b_arr, b_arr_len);

    node_content_t res_arr[] = { { 0, 0, 4 }, { 0, 1, 6 }, { 2, 1, 3 } };
    size_t res_arr_len = sizeof(res_arr) / sizeof(res_arr[0]);

    mnode_t *res = mlist_add(a, b);

    cmp_list_and_arr(res, res_arr, res_arr_len);

    mlist_free(&a);
    mlist_free(&b);
    mlist_free(&res);
}
END_TEST

START_TEST(test_mlist_add_null_pointer)
{
    ck_assert_ptr_eq(mlist_add(NULL, NULL), NULL);
}
END_TEST

Suite* mlist_add_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_mlist_add_null_pointer);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_mlist_add_usual);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_mlist_del_row_max_usual)
{
    node_content_t a_arr[] = { { 0, 0, 1 }, { 0, 1, 3 }, { 1, 0, 5 }, { 1, 3, 4 }, { 2, 0, 1 } };
    size_t a_arr_len = sizeof(a_arr) / sizeof(a_arr[0]);
    mnode_t *a = list_from_arr(a_arr, a_arr_len);

    node_content_t res_arr[] = { { 0, 0, 1 }, { 0, 1, 3 }, { 1, 0, 1 } };
    size_t res_arr_len = sizeof(res_arr) / sizeof(res_arr[0]);

    ck_assert_int_eq(mlist_del_row_max(&a), EXIT_SUCCESS);

    cmp_list_and_arr(a, res_arr, res_arr_len);

    mlist_free(&a);
}
END_TEST

START_TEST(test_mlist_del_row_max_null_pointer)
{
    ck_assert_int_ne(mlist_del_row_max(NULL), EXIT_SUCCESS);
}
END_TEST

Suite* mlist_del_row_max_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_mlist_del_row_max_null_pointer);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_mlist_del_row_max_usual);
    suite_add_tcase(s, tc_pos);

    return s;
}
