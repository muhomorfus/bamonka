#include <check.h>
#include <stdlib.h>

#include "queue.h"

void cmp_queue_and_arr(queue_t *q, int *arr, size_t n)
{
    size_t i = 0;
    int buf;
    while (!queue_pop(q, &buf))
    {
        ck_assert_int_eq(arr[i++], buf);
    }

    ck_assert_int_eq(i, n);
}

START_TEST(test_queue_push_one_element)
    {
        queue_t q = { 0 };
        int err = queue_push(&q, 1);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        int res[] = { 1 };
        cmp_queue_and_arr(&q, res, sizeof(res) / sizeof(res[0]));
        queue_free(&q);
    }
END_TEST

START_TEST(test_queue_push_two_elements)
    {
        queue_t q = { 0 };
        int err = queue_push(&q, 1);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        err = queue_push(&q, 2);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        int res[] = { 1, 2 };
        cmp_queue_and_arr(&q, res, sizeof(res) / sizeof(res[0]));
        queue_free(&q);
    }
END_TEST

START_TEST(test_queue_push_null_pointer)
    {
        int err = queue_push(NULL, 1);
        ck_assert_int_ne(err, EXIT_SUCCESS);
    }
END_TEST

Suite* queue_push_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_queue_push_null_pointer);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_neg, test_queue_push_one_element);
    tcase_add_test(tc_neg, test_queue_push_two_elements);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_queue_pop_was_one_element)
    {
        queue_t q = { 0 };
        int err = queue_push(&q, 10);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        int buf;
        err = queue_pop(&q, &buf);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        ck_assert_int_eq(buf, 10);
        queue_free(&q);
    }
END_TEST

START_TEST(test_queue_pop_was_many_elements)
    {
        queue_t q = { 0 };
        int err = queue_push(&q, 10);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        err = queue_push(&q, 8);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        err = queue_push(&q, 5);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        err = queue_push(&q, 2);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        err = queue_push(&q, 19);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        int buf;
        err = queue_pop(&q, &buf);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        int res[] = { 8, 5, 2, 19 };
        cmp_queue_and_arr(&q, res, sizeof(res) / sizeof(res[0]));
        queue_free(&q);
    }
END_TEST

START_TEST(test_queue_pop_null_pointer)
    {
        queue_t q = { 0 };
        int buf;
        int err = queue_push(&q, 10);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        err = queue_pop(NULL, &buf);
        ck_assert_int_ne(err, EXIT_SUCCESS);

        err = queue_pop(&q, NULL);
        ck_assert_int_ne(err, EXIT_SUCCESS);
        queue_free(&q);
    }
END_TEST

START_TEST(test_queue_pop_empty)
    {
        queue_t q = { 0 };
        int buf;

        int err = queue_pop(&q, &buf);
        ck_assert_int_ne(err, EXIT_SUCCESS);
    }
END_TEST

Suite* queue_pop_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_queue_pop_null_pointer);
    tcase_add_test(tc_neg, test_queue_pop_empty);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_neg, test_queue_pop_was_one_element);
    tcase_add_test(tc_neg, test_queue_pop_was_many_elements);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_queue_push_many_one_element)
    {
        queue_t q = { 0 };
        int err = queue_push_many(&q, 1, 1);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        int res[] = { 1 };
        cmp_queue_and_arr(&q, res, sizeof(res) / sizeof(res[0]));
        queue_free(&q);
    }
END_TEST

START_TEST(test_queue_push_many_two_elements)
    {
        queue_t q = { 0 };
        int err = queue_push_many(&q, 2, 1, 2);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        int res[] = { 1, 2 };
        cmp_queue_and_arr(&q, res, sizeof(res) / sizeof(res[0]));
        queue_free(&q);
    }
END_TEST

START_TEST(test_queue_push_many_many_elements)
    {
        queue_t q = { 0 };
        int err = queue_push_many(&q, 10, 1, 4, 5, 2, 3, 4, 5, 10, 22, 30);
        ck_assert_int_eq(err, EXIT_SUCCESS);

        int res[] = { 1, 4, 5, 2, 3, 4, 5, 10, 22, 30 };
        cmp_queue_and_arr(&q, res, sizeof(res) / sizeof(res[0]));
        queue_free(&q);
    }
END_TEST

START_TEST(test_queue_push_many_null_pointer)
    {
        int err = queue_push_many(NULL, 1, 1);
        ck_assert_int_ne(err, EXIT_SUCCESS);
    }
END_TEST

Suite* queue_push_many_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_queue_push_many_null_pointer);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_neg, test_queue_push_many_one_element);
    tcase_add_test(tc_neg, test_queue_push_many_two_elements);
    tcase_add_test(tc_neg, test_queue_push_many_many_elements);
    suite_add_tcase(s, tc_pos);

    return s;
}

