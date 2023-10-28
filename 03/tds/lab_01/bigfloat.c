#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "bigfloat.h"
#include "bigint.h"
#include "error.h"
#include "short_string.h"

bigfloat_t bf_create()
{
    bigfloat_t a = { bi_create(), 0 };
    return a;
}

error_t __parse_significant(char *begin, char *end, bigint_t *sign, int *to_exp)
{
    if (!begin || !end || !sign || !to_exp)
        return NEW_ERROR("Передан нулевой указатель", BF_ERROR_NULL_POINTER);
    if (begin >= end)
        return NEW_ERROR("Указатели накладываются", BF_ERROR_EQUAL_POINTERS);

    *to_exp = 0;
    *sign = bi_create();
    sign->sign = 1;
    size_t j = BIG_FLOAT_SIZE;
    int s = 0;
    int digits = 0;
    if (*begin == '-' || *begin == '+')
    {
        if (*begin == '-')
            sign->sign = -1;
        begin++;
    }

    char *ptr;
    for (ptr = begin; ptr < end && j > 0 && *ptr != '.'; ptr++)
    {
        digit d = char_to_digit(*ptr);
        if (d < 0)
            return NEW_ERROR("Некорретная целая часть", BF_ERROR_INVALID_STR);
        digits++;
        if (d != 0 || s > 0)
        {
            sign->content[j - 1] = d;
            j--;
            s++;
        }
    }

    *to_exp += s;
    for (ptr++; ptr < end && j > 0; ptr++)
    {
        digit d = char_to_digit(*ptr);
        if (d < 0)
            return NEW_ERROR("Некорретная дробная часть", BF_ERROR_INVALID_STR);
        digits++;
        if (d != 0 || s > 0)
        {
            sign->content[j - 1] = d;
            j--;
            s++;
        }
        if (s == 0)
            (*to_exp)--;
    }

    if (j == 0 && ptr < end)
        return NEW_ERROR("Мантисса слишком большая", BF_ERROR_OVERFLOW);

    if (!digits)
        return NEW_ERROR("Мантисса пуста", BF_ERROR_INVALID_STR);

    if (!s && digits)
        sign->sign = 0;

    return NEW_SUCCESS();
}

error_t __parse_exponent(char *begin, char *end, int *exp)
{
    if (!begin || !end || !exp)
        return NEW_ERROR("Передан нулевой указатель", BF_ERROR_NULL_POINTER);
    if (begin == end)
        return NEW_ERROR("Указатели накладываются", BF_ERROR_EQUAL_POINTERS);

    *exp = 0;
    int sign = 1;
    if (*begin == '+' || *begin == '-')
    {
        if (*begin == '-')
            sign = -1;
        begin++;
    }

    int digits = 0;
    for (char *ptr = begin; ptr < end; ptr++)
    {
        digit d = char_to_digit(*ptr);
        if (d < 0)
            return NEW_ERROR("Некорретная степень", BF_ERROR_INVALID_STR);
        digits++;
        *exp = *exp * 10 + d;
    }

    if (!digits)
        return NEW_ERROR("Ступень пуста", BF_ERROR_INVALID_STR);

    *exp *= sign;
    return NEW_SUCCESS();
}

bool __is_exponent_overflow(bigfloat_t a)
{
    return a.exponent > BIG_FLOAT_MAX_EXPONENT || a.exponent < BIG_FLOAT_MIN_EXPONENT;
}

error_t bf_from_str(char *str, bigfloat_t *a)
{
    if (!str || !a)
        return NEW_ERROR("Передан нулевой указатель", BF_ERROR_NULL_POINTER);

    str = normalize_str(str);
    size_t len = strlen(str);
    *a = bf_create();

    char *e_pos = strchr(str, EXP_SYMBOL);
    error_t err;
    int to_exp = 0;
    if (!e_pos)
    {
        err = __parse_significant(str, str + len, &a->significand, &to_exp);
        if (is_failure(err))
            return err;
    }
    else
    {
        err = __parse_significant(str, e_pos, &a->significand, &to_exp);
        if (is_failure(err))
            return err;

        err = __parse_exponent(e_pos + 1, str + len, &a->exponent);
        if (is_failure(err))
            return err;
    }

    if (__is_exponent_overflow(*a))
        return NEW_ERROR("Переполнение экспоненты", BF_ERROR_OVERFLOW);
    a->exponent += to_exp;

    return NEW_SUCCESS();
}

error_t bf_to_str(char *str, bigfloat_t a)
{
    if (!str)
        return NEW_ERROR("Передан нулевой указатель", BF_ERROR_NULL_POINTER);

    if (a.significand.sign < 0)
        *(str++) = '-';
    else if (a.significand.sign >= 0)
        *(str++) = '+';
    *(str++) = '0';
    *(str++) = '.';

    size_t last;
    for (last = 0; last < BIG_FLOAT_SIZE && !a.significand.content[last]; last++);

    for (size_t i = BIG_FLOAT_SIZE; i > last; i--)
        *(str++) = digit_to_char(a.significand.content[i - 1]);

    if (last == BIG_FLOAT_SIZE)
        *(str++) = '0';

    *(str++) = 'E';
    sprintf(str, "%+d", a.exponent);

    return NEW_SUCCESS();
}

void __to_normal(bigfloat_t *a)
{
    if (!a)
        return;

    size_t i;
    for (i = BIG_INT_SIZE; i > 0 && !a->significand.content[i - 1]; i--);

    int shift = (int) i - BIG_FLOAT_SIZE;
    bi_rshift_rounded(&a->significand, shift);
    if (a->significand.content[BIG_FLOAT_SIZE] != 0)
    {
        bi_rshift(&a->significand, 1);
        shift++;
    }

    a->exponent += shift;
}

error_t bf_div(bigfloat_t *a, bigfloat_t b)
{
    if (!a)
        return NEW_ERROR("Передан нулевой указатель", BF_ERROR_NULL_POINTER);

    if (!a->significand.sign && b.significand.sign)
    {
        *a = bf_create();
        return NEW_SUCCESS();
    }

    bi_lshift(&a->significand, BF_COMPUTE_SHIFT);

    error_t err = bi_div(&a->significand, b.significand);
    a->exponent -= b.exponent;
    if (is_failure(err))
        return err;

    __to_normal(a);
    if (__is_exponent_overflow(*a))
        return NEW_ERROR("Переполнение экспоненты", BF_ERROR_OVERFLOW);

    return NEW_SUCCESS();
}

error_t bf_from_bigint(bigint_t n, bigfloat_t *a)
{
    if (!a)
        return NEW_ERROR("Передан нулевой указатель", BF_ERROR_NULL_POINTER);

    bigint_t border = bi_from_ll(1);
    bi_lshift(&border, BIG_FLOAT_SIZE);
    if (bi_cmp(n, border) >= 0)
        return NEW_ERROR("Слишком большое целое число.", BF_ERROR_OVERFLOW);

    *a = bf_create();
    a->significand = n;
    __to_normal(a);
    a->exponent += BIG_FLOAT_SIZE;
    if (__is_exponent_overflow(*a))
        return NEW_ERROR("Переполнение экспоненты", BF_ERROR_OVERFLOW);

    return NEW_SUCCESS();
}