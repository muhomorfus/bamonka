#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <check.h>
#include <limits.h>

#include "my_snprintf.h"

void test_snprintf(size_t n, const char *restrict format, ...)
{
    va_list arg;
    va_list my_arg;

    va_start(arg, format);
    va_start(my_arg, format);

    char *str = NULL;
    char *my_str = NULL;
    if (n)
    {
        str = malloc(n * sizeof(char));
        my_str = malloc(n * sizeof(char));
        for (size_t i = 0; i < n; i++)
        {
            str[i] = 1;
            my_str[i] = 2;
        }
        ck_assert_ptr_ne(str, NULL);
        ck_assert_ptr_ne(my_str, NULL);
    }

    int code = vsnprintf(str, n, format, arg);
    int my_code = my_vsnprintf(my_str, n, format, my_arg);
    ck_assert_int_eq(code, my_code);
    if (n)
        ck_assert_str_eq(str, my_str);

    va_end(my_arg);
    va_end(arg);

    free(str);
    free(my_str);
}

START_TEST(test_my_snprintf_i_usual)
{
    test_snprintf(10, "%i", 10);
    test_snprintf(10, "%d", 10);
}
END_TEST

START_TEST(test_my_snprintf_i_negative)
    {
        test_snprintf(10, "%i", -10);
        test_snprintf(10, "%d", -10);
    }
END_TEST

START_TEST(test_my_snprintf_i_with_text)
    {
        test_snprintf(10, "Hello, %i", -10);
        test_snprintf(10, "Hello, %d", -10);
    }
END_TEST

START_TEST(test_my_snprintf_i_multiple)
    {
        test_snprintf(100, "Hello, %i, my name is %i. Ok, %i\n", -10, 20, 4);
        test_snprintf(100, "Hello, %d, my name is %d. Ok, %d\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_i_overflow)
    {
        test_snprintf(9, "Hello, %i, my name is %i. Ok, %i\n", -10, 20, 4);
        test_snprintf(9, "Hello, %d, my name is %d. Ok, %d\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_i_multiple_in_a_row)
    {
        test_snprintf(20, "Hello, %i%i%i\n", -10, 20, 4);
        test_snprintf(20, "Hello, %d%d%d\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_i_max)
    {
        test_snprintf(40, "Hello, %i%i%i\n", INT_MAX, 20, 4);
        test_snprintf(40, "Hello, %d%d%d\n", INT_MAX, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_i_min)
    {
        test_snprintf(40, "Hello, %i%i%i\n", INT_MIN, 20, 4);
        test_snprintf(40, "Hello, %d%d%d\n", INT_MIN, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_i_zero_symbols)
    {
        test_snprintf(0, "Hello, %i%i%i\n", INT_MIN, 20, 4);
        test_snprintf(0, "Hello, %d%d%d\n", INT_MIN, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_li_usual)
    {
        test_snprintf(10, "%li", 10);
        test_snprintf(10, "%ld", 10);
    }
END_TEST

START_TEST(test_my_snprintf_li_negative)
    {
        test_snprintf(10, "%li", -10);
        test_snprintf(10, "%ld", -10);
    }
END_TEST

START_TEST(test_my_snprintf_li_with_text)
    {
        test_snprintf(10, "Hello, %li", -10);
        test_snprintf(10, "Hello, %ld", -10);
    }
END_TEST

START_TEST(test_my_snprintf_li_multiple)
    {
        test_snprintf(100, "Hello, %li, my name is %li. Ok, %li\n", -10, 20, 4);
        test_snprintf(100, "Hello, %ld, my name is %ld. Ok, %ld\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_li_overflow)
    {
        test_snprintf(9, "Hello, %li, my name is %li. Ok, %li\n", -10, 20, 4);
        test_snprintf(9, "Hello, %ld, my name is %ld. Ok, %ld\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_li_multiple_in_a_row)
    {
        test_snprintf(20, "Hello, %li%li%li\n", -10, 20, 4);
        test_snprintf(20, "Hello, %ld%ld%ld\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_li_max)
    {
        test_snprintf(40, "Hello, %li%li%li\n", LONG_MAX, 20, 4);
        test_snprintf(40, "Hello, %ld%ld%ld\n", LONG_MAX, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_li_min)
    {
        test_snprintf(40, "Hello, %li%li%li\n", LONG_MIN, 20, 4);
        test_snprintf(40, "Hello, %ld%ld%ld\n", LONG_MIN, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_li_zero_symbols)
    {
        test_snprintf(0, "Hello, %li%li%li\n", LONG_MIN, 20, 4);
        test_snprintf(0, "Hello, %ld%ld%ld\n", LONG_MIN, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_hi_usual)
    {
        test_snprintf(10, "%hi", 10);
        test_snprintf(10, "%hd", 10);
    }
END_TEST

START_TEST(test_my_snprintf_hi_negative)
    {
        test_snprintf(10, "%hi", -10);
        test_snprintf(10, "%hd", -10);
    }
END_TEST

START_TEST(test_my_snprintf_hi_with_text)
    {
        test_snprintf(10, "Hello, %hi", -10);
        test_snprintf(10, "Hello, %hd", -10);
    }
END_TEST

START_TEST(test_my_snprintf_hi_multiple)
    {
        test_snprintf(100, "Hello, %hi, my name is %hi. Ok, %hi\n", -10, 20, 4);
        test_snprintf(100, "Hello, %hd, my name is %hd. Ok, %hd\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_hi_overflow)
    {
        test_snprintf(9, "Hello, %hi, my name is %hi. Ok, %hi\n", -10, 20, 4);
        test_snprintf(9, "Hello, %hd, my name is %hd. Ok, %hd\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_hi_multiple_in_a_row)
    {
        test_snprintf(20, "Hello, %hi%hi%hi\n", -10, 20, 4);
        test_snprintf(20, "Hello, %hd%hd%hd\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_hi_max)
    {
        test_snprintf(40, "Hello, %hi%hi%hi\n", SHRT_MAX, 20, 4);
        test_snprintf(40, "Hello, %hd%hd%hd\n", SHRT_MAX, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_hi_min)
    {
        test_snprintf(40, "Hello, %hi%hi%hi\n", SHRT_MIN, 20, 4);
        test_snprintf(40, "Hello, %hd%hd%hd\n", SHRT_MIN, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_hi_zero_symbols)
    {
        test_snprintf(0, "Hello, %hi%hi%hi\n", SHRT_MIN, 20, 4);
        test_snprintf(0, "Hello, %hd%hd%hd\n", SHRT_MIN, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_hx_usual)
    {
        test_snprintf(10, "%hx", 10);
    }
END_TEST

START_TEST(test_my_snprintf_hx_negative)
    {
        test_snprintf(10, "%hx", -10);
    }
END_TEST

START_TEST(test_my_snprintf_hx_with_text)
    {
        test_snprintf(10, "Hello, %hx", -10);
    }
END_TEST

START_TEST(test_my_snprintf_hx_multiple)
    {
        test_snprintf(100, "Hello, %hx, my name is %hx. Ok, %hx\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_hx_overflow)
    {
        test_snprintf(9, "Hello, %hx, my name is %hx. Ok, %hx\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_hx_multiple_in_a_row)
    {
        test_snprintf(20, "Hello, %hx%hx%hx\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_hx_max)
    {
        test_snprintf(40, "Hello, %hx%hx%hx\n", USHRT_MAX, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_hx_min)
    {
        test_snprintf(40, "Hello, %hx%hx%hx\n", 0, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_hx_zero_symbols)
    {
        test_snprintf(0, "Hello, %hx%hx%hx\n", 0, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_ho_usual)
    {
        test_snprintf(10, "%ho", 10);
    }
END_TEST

START_TEST(test_my_snprintf_ho_negative)
    {
        test_snprintf(10, "%ho", -10);
    }
END_TEST

START_TEST(test_my_snprintf_ho_with_text)
    {
        test_snprintf(10, "Hello, %ho", -10);
    }
END_TEST

START_TEST(test_my_snprintf_ho_multiple)
    {
        test_snprintf(100, "Hello, %ho, my name is %ho. Ok, %ho\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_ho_overflow)
    {
        test_snprintf(9, "Hello, %ho, my name is %ho. Ok, %ho\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_ho_multiple_in_a_row)
    {
        test_snprintf(20, "Hello, %ho%ho%ho\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_ho_max)
    {
        test_snprintf(40, "Hello, %ho%ho%ho\n", USHRT_MAX, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_ho_min)
    {
        test_snprintf(40, "Hello, %ho%ho%ho\n", 0, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_ho_zero_symbols)
    {
        test_snprintf(0, "Hello, %ho%ho%ho\n", 0, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_lx_usual)
    {
        test_snprintf(10, "%lx", 10);
    }
END_TEST

START_TEST(test_my_snprintf_lx_negative)
    {
        test_snprintf(10, "%lx", -10);
    }
END_TEST

START_TEST(test_my_snprintf_lx_with_text)
    {
        test_snprintf(10, "Hello, %lx", -10);
    }
END_TEST

START_TEST(test_my_snprintf_lx_multiple)
    {
        test_snprintf(100, "Hello, %lx, my name is %lx. Ok, %lx\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_lx_overflow)
    {
        test_snprintf(9, "Hello, %lx, my name is %lx. Ok, %lx\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_lx_multiple_in_a_row)
    {
        test_snprintf(20, "Hello, %lx%lx%lx\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_lx_max)
    {
        test_snprintf(40, "Hello, %lx%lx%lx\n", ULONG_MAX, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_lx_min)
    {
        test_snprintf(40, "Hello, %lx%lx%lx\n", 0, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_lx_zero_symbols)
    {
        test_snprintf(0, "Hello, %lx%lx%lx\n", 0, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_lo_usual)
    {
        test_snprintf(10, "%lo", 10);
    }
END_TEST

START_TEST(test_my_snprintf_lo_negative)
    {
        test_snprintf(10, "%lo", -10);
    }
END_TEST

START_TEST(test_my_snprintf_lo_with_text)
    {
        test_snprintf(10, "Hello, %lo", -10);
    }
END_TEST

START_TEST(test_my_snprintf_lo_multiple)
    {
        test_snprintf(100, "Hello, %lo, my name is %lo. Ok, %lo\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_lo_overflow)
    {
        test_snprintf(9, "Hello, %lo, my name is %lo. Ok, %lo\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_lo_multiple_in_a_row)
    {
        test_snprintf(20, "Hello, %lo%lo%lo\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_lo_max)
    {
        test_snprintf(40, "Hello, %lo%lo%lo\n", ULONG_MAX, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_lo_min)
    {
        test_snprintf(40, "Hello, %lo%lo%lo\n", 0, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_lo_zero_symbols)
    {
        test_snprintf(0, "Hello, %lo%lo%lo\n", 0, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_x_usual)
    {
        test_snprintf(10, "%x", 10);
    }
END_TEST

START_TEST(test_my_snprintf_x_negative)
    {
        test_snprintf(10, "%x", -10);
    }
END_TEST

START_TEST(test_my_snprintf_x_with_text)
    {
        test_snprintf(10, "Hello, %x", -10);
    }
END_TEST

START_TEST(test_my_snprintf_x_multiple)
    {
        test_snprintf(100, "Hello, %x, my name is %x. Ok, %x\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_x_overflow)
    {
        test_snprintf(9, "Hello, %x, my name is %x. Ok, %x\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_x_multiple_in_a_row)
    {
        test_snprintf(20, "Hello, %x%x%x\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_x_max)
    {
        test_snprintf(40, "Hello, %x%x%x\n", UINT_MAX, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_x_min)
    {
        test_snprintf(40, "Hello, %x%x%x\n", 0, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_x_zero_symbols)
    {
        test_snprintf(0, "Hello, %x%x%x\n", 0, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_o_usual)
    {
        test_snprintf(10, "%o", 10);
    }
END_TEST

START_TEST(test_my_snprintf_o_negative)
    {
        test_snprintf(10, "%o", -10);
    }
END_TEST

START_TEST(test_my_snprintf_o_with_text)
    {
        test_snprintf(10, "Hello, %o", -10);
    }
END_TEST

START_TEST(test_my_snprintf_o_multiple)
    {
        test_snprintf(100, "Hello, %o, my name is %o. Ok, %o\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_o_overflow)
    {
        test_snprintf(9, "Hello, %o, my name is %o. Ok, %o\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_o_multiple_in_a_row)
    {
        test_snprintf(20, "Hello, %o%o%o\n", -10, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_o_max)
    {
        test_snprintf(40, "Hello, %o%o%o\n", UINT_MAX, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_o_min)
    {
        test_snprintf(40, "Hello, %o%o%o\n", 0, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_o_zero_symbols)
    {
        test_snprintf(0, "Hello, %o%o%o\n", 0, 20, 4);
    }
END_TEST

START_TEST(test_my_snprintf_s_zero_str)
    {
        test_snprintf(10, "%s", "");
    }
END_TEST

START_TEST(test_my_snprintf_s_usual)
    {
        test_snprintf(10, "%s", "hello");
    }
END_TEST

START_TEST(test_my_snprintf_s_overflow)
    {
        test_snprintf(3, "%s", "hello world my name is alex");
    }
END_TEST

START_TEST(test_my_snprintf_s_multiple)
    {
        test_snprintf(20, "%s %s, %s", "hello", "you", "bred228");
    }
END_TEST

START_TEST(test_my_snprintf_s_one)
    {
        test_snprintf(1, "%s %s, %s", "hello", "you", "bred228");
    }
END_TEST

START_TEST(test_my_snprintf_c_usual)
    {
        test_snprintf(10, "%c", 'h');
    }
END_TEST

START_TEST(test_my_snprintf_c_multiple)
    {
        test_snprintf(10, "%c%c%c", 'h', 'e', 'y');
    }
END_TEST

START_TEST(test_my_snprintf_c_overflow)
    {
        test_snprintf(2, "%c%c%c", 'h', 'e', 'y');
    }
END_TEST

START_TEST(test_my_snprintf_c_with_text)
    {
        test_snprintf(2, "%cello", 'h');
    }
END_TEST

Suite* my_snprintf_suite(Suite *s)
{
    TCase *tc_pos;

    tc_pos = tcase_create("pos");

    suite_add_tcase(s, tc_pos);

    tc_pos = tcase_create("pos");
    tcase_add_test(tc_pos, test_my_snprintf_i_usual);
    tcase_add_test(tc_pos, test_my_snprintf_i_negative);
    tcase_add_test(tc_pos, test_my_snprintf_i_with_text);
    tcase_add_test(tc_pos, test_my_snprintf_i_multiple);
    tcase_add_test(tc_pos, test_my_snprintf_i_overflow);
    tcase_add_test(tc_pos, test_my_snprintf_i_multiple_in_a_row);
    tcase_add_test(tc_pos, test_my_snprintf_i_max);
    tcase_add_test(tc_pos, test_my_snprintf_i_min);
    tcase_add_test(tc_pos, test_my_snprintf_i_zero_symbols);

    tcase_add_test(tc_pos, test_my_snprintf_li_usual);
    tcase_add_test(tc_pos, test_my_snprintf_li_negative);
    tcase_add_test(tc_pos, test_my_snprintf_li_with_text);
    tcase_add_test(tc_pos, test_my_snprintf_li_multiple);
    tcase_add_test(tc_pos, test_my_snprintf_li_overflow);
    tcase_add_test(tc_pos, test_my_snprintf_li_multiple_in_a_row);
    tcase_add_test(tc_pos, test_my_snprintf_li_max);
    tcase_add_test(tc_pos, test_my_snprintf_li_min);
    tcase_add_test(tc_pos, test_my_snprintf_li_zero_symbols);

    tcase_add_test(tc_pos, test_my_snprintf_hi_usual);
    tcase_add_test(tc_pos, test_my_snprintf_hi_negative);
    tcase_add_test(tc_pos, test_my_snprintf_hi_with_text);
    tcase_add_test(tc_pos, test_my_snprintf_hi_multiple);
    tcase_add_test(tc_pos, test_my_snprintf_hi_overflow);
    tcase_add_test(tc_pos, test_my_snprintf_hi_multiple_in_a_row);
    tcase_add_test(tc_pos, test_my_snprintf_hi_max);
    tcase_add_test(tc_pos, test_my_snprintf_hi_min);
    tcase_add_test(tc_pos, test_my_snprintf_hi_zero_symbols);

    tcase_add_test(tc_pos, test_my_snprintf_hx_usual);
    tcase_add_test(tc_pos, test_my_snprintf_hx_negative);
    tcase_add_test(tc_pos, test_my_snprintf_hx_with_text);
    tcase_add_test(tc_pos, test_my_snprintf_hx_multiple);
    tcase_add_test(tc_pos, test_my_snprintf_hx_overflow);
    tcase_add_test(tc_pos, test_my_snprintf_hx_multiple_in_a_row);
    tcase_add_test(tc_pos, test_my_snprintf_hx_max);
    tcase_add_test(tc_pos, test_my_snprintf_hx_min);
    tcase_add_test(tc_pos, test_my_snprintf_hx_zero_symbols);

    tcase_add_test(tc_pos, test_my_snprintf_ho_usual);
    tcase_add_test(tc_pos, test_my_snprintf_ho_negative);
    tcase_add_test(tc_pos, test_my_snprintf_ho_with_text);
    tcase_add_test(tc_pos, test_my_snprintf_ho_multiple);
    tcase_add_test(tc_pos, test_my_snprintf_ho_overflow);
    tcase_add_test(tc_pos, test_my_snprintf_ho_multiple_in_a_row);
    tcase_add_test(tc_pos, test_my_snprintf_ho_max);
    tcase_add_test(tc_pos, test_my_snprintf_ho_min);
    tcase_add_test(tc_pos, test_my_snprintf_ho_zero_symbols);

    tcase_add_test(tc_pos, test_my_snprintf_lx_usual);
    tcase_add_test(tc_pos, test_my_snprintf_lx_negative);
    tcase_add_test(tc_pos, test_my_snprintf_lx_with_text);
    tcase_add_test(tc_pos, test_my_snprintf_lx_multiple);
    tcase_add_test(tc_pos, test_my_snprintf_lx_overflow);
    tcase_add_test(tc_pos, test_my_snprintf_lx_multiple_in_a_row);
    tcase_add_test(tc_pos, test_my_snprintf_lx_max);
    tcase_add_test(tc_pos, test_my_snprintf_lx_min);
    tcase_add_test(tc_pos, test_my_snprintf_lx_zero_symbols);

    tcase_add_test(tc_pos, test_my_snprintf_lo_usual);
    tcase_add_test(tc_pos, test_my_snprintf_lo_negative);
    tcase_add_test(tc_pos, test_my_snprintf_lo_with_text);
    tcase_add_test(tc_pos, test_my_snprintf_lo_multiple);
    tcase_add_test(tc_pos, test_my_snprintf_lo_overflow);
    tcase_add_test(tc_pos, test_my_snprintf_lo_multiple_in_a_row);
    tcase_add_test(tc_pos, test_my_snprintf_lo_max);
    tcase_add_test(tc_pos, test_my_snprintf_lo_min);
    tcase_add_test(tc_pos, test_my_snprintf_lo_zero_symbols);

    tcase_add_test(tc_pos, test_my_snprintf_x_usual);
    tcase_add_test(tc_pos, test_my_snprintf_x_negative);
    tcase_add_test(tc_pos, test_my_snprintf_x_with_text);
    tcase_add_test(tc_pos, test_my_snprintf_x_multiple);
    tcase_add_test(tc_pos, test_my_snprintf_x_overflow);
    tcase_add_test(tc_pos, test_my_snprintf_x_multiple_in_a_row);
    tcase_add_test(tc_pos, test_my_snprintf_x_max);
    tcase_add_test(tc_pos, test_my_snprintf_x_min);
    tcase_add_test(tc_pos, test_my_snprintf_x_zero_symbols);

    tcase_add_test(tc_pos, test_my_snprintf_o_usual);
    tcase_add_test(tc_pos, test_my_snprintf_o_negative);
    tcase_add_test(tc_pos, test_my_snprintf_o_with_text);
    tcase_add_test(tc_pos, test_my_snprintf_o_multiple);
    tcase_add_test(tc_pos, test_my_snprintf_o_overflow);
    tcase_add_test(tc_pos, test_my_snprintf_o_multiple_in_a_row);
    tcase_add_test(tc_pos, test_my_snprintf_o_max);
    tcase_add_test(tc_pos, test_my_snprintf_o_min);
    tcase_add_test(tc_pos, test_my_snprintf_o_zero_symbols);

    tcase_add_test(tc_pos, test_my_snprintf_s_zero_str);
    tcase_add_test(tc_pos, test_my_snprintf_s_usual);
    tcase_add_test(tc_pos, test_my_snprintf_s_overflow);
    tcase_add_test(tc_pos, test_my_snprintf_s_multiple);
    tcase_add_test(tc_pos, test_my_snprintf_s_one);

    tcase_add_test(tc_pos, test_my_snprintf_c_usual);
    tcase_add_test(tc_pos, test_my_snprintf_c_overflow);
    tcase_add_test(tc_pos, test_my_snprintf_c_multiple);
    tcase_add_test(tc_pos, test_my_snprintf_c_with_text);

    suite_add_tcase(s, tc_pos);

    return s;
}
