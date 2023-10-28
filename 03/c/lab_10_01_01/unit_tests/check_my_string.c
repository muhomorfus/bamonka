#include <stdlib.h>
#include <check.h>

#include "my_string.h"

START_TEST(test_to_double_normal_form)
{
    double n;
    ck_assert_int_eq(to_double("1.234", &n), 0);
}
END_TEST

START_TEST(test_to_double_exp_form)
{
    double n;
    ck_assert_int_eq(0, to_double("1.234e3", &n));
}
END_TEST

START_TEST(test_to_double_with_sign)
{
    double n;
    ck_assert_int_eq(0, to_double("-1.234", &n));
}
END_TEST

START_TEST(test_to_double_int_number)
{
    double n;
    ck_assert_int_eq(0, to_double("228", &n));
}
END_TEST

START_TEST(test_to_double_int_number_with_sign)
{
    double n;
    ck_assert_int_eq(0, to_double("-228", &n));
}
END_TEST

START_TEST(test_to_double_zero)
{
    double n;
    ck_assert_int_eq(0, to_double("0", &n));
}
END_TEST

START_TEST(test_to_double_letters)
{
    double n;
    ck_assert_int_ne(0, to_double("abs", &n));
}
END_TEST

START_TEST(test_to_double_many_dots)
{
    double n;
    ck_assert_int_ne(0, to_double("12.11.121", &n));
}
END_TEST

START_TEST(test_to_double_many_signs)
{
    double n;
    ck_assert_int_ne(0, to_double("+-1.22", &n));
}
END_TEST

START_TEST(test_to_double_with_letters)
{
    double n;
    ck_assert_int_ne(0, to_double("1.22ff4", &n));
}
END_TEST

START_TEST(test_to_double_null)
{
    double n;
    ck_assert_int_ne(0, to_double(NULL, &n));
}
END_TEST

Suite* to_double_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_to_double_letters);
    tcase_add_test(tc_neg, test_to_double_many_dots);
    tcase_add_test(tc_neg, test_to_double_many_signs);
    tcase_add_test(tc_neg, test_to_double_with_letters);
    tcase_add_test(tc_neg, test_to_double_null);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_to_double_normal_form);
    tcase_add_test(tc_pos, test_to_double_exp_form);
    tcase_add_test(tc_pos, test_to_double_with_sign);
    tcase_add_test(tc_pos, test_to_double_int_number);
    tcase_add_test(tc_pos, test_to_double_int_number_with_sign);
    tcase_add_test(tc_pos, test_to_double_zero);
    suite_add_tcase(s, tc_pos);

    return s;
}

START_TEST(test_to_integer_normal)
{
    int n;
    ck_assert_int_eq(0, to_integer("1234", &n));
}
END_TEST

START_TEST(test_to_integer_signed)
{
    int n;
    ck_assert_int_eq(0, to_integer("-1234", &n));
}
END_TEST

START_TEST(test_to_integer_zero)
{
    int n;
    ck_assert_int_eq(0, to_integer("0", &n));
}
END_TEST

START_TEST(test_to_integer_letters)
{
    int n;
    ck_assert_int_ne(0, to_integer("ahshs", &n));
}
END_TEST

START_TEST(test_to_integer_with_letters)
{
    int n;
    ck_assert_int_ne(0, to_integer("12n223", &n));
}
END_TEST

START_TEST(test_to_integer_null)
{
    int n;
    ck_assert_int_ne(0, to_integer(NULL, &n));
}
END_TEST

Suite* to_integer_suite(Suite *s)
{
    TCase *tc_neg;
    TCase *tc_pos;

    tc_neg = tcase_create("neg");
    tcase_add_test(tc_neg, test_to_integer_letters);
    tcase_add_test(tc_neg, test_to_integer_with_letters);
    tcase_add_test(tc_neg, test_to_integer_null);
    suite_add_tcase(s, tc_neg);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_to_integer_normal);
    tcase_add_test(tc_pos, test_to_integer_signed);
    tcase_add_test(tc_pos, test_to_integer_zero);
    suite_add_tcase(s, tc_pos);

    return s;
}
