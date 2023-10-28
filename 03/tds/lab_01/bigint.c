#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "bigint.h"
#include "error.h"
#include "short_string.h"

void __bi_print(bigint_t a)
{
    if (a.sign < 0)
        printf("-");

    for (size_t i = BIG_INT_SIZE; i > 0; i--)
        printf("%d", a.content[i - 1]);

    printf("\n");
}

char symbols[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

bigint_t bi_create()
{
    bigint_t a = {0};
    return a;
}

bigint_t bi_from_ll(long long n)
{
    bigint_t a = bi_create();

    if (n == 0)
        return a;

    if (n < 0)
    {
        n = -n;
        a.sign = -1;
    } else
        a.sign = 1;

    for (size_t i = 0; i < BIG_INT_SIZE && n != 0; i++)
    {
        a.content[i] = (digit) (n % BIG_INT_BASE);
        n /= BIG_INT_BASE;
    }

    return a;
}

int __abs_cmp(bigint_t a, bigint_t b)
{
    for (size_t i = BIG_INT_SIZE; i > 0; i--)
        if (a.content[i - 1] != b.content[i - 1])
            return a.content[i - 1] - b.content[i - 1];

    return 0;
}

int bi_cmp(bigint_t a, bigint_t b)
{
    if (a.sign != b.sign)
        return a.sign - b.sign;

    int abs_cmp = __abs_cmp(a, b);
    if (a.sign < 0)
        return -abs_cmp;

    return abs_cmp;
}

bigint_t bi_neg(bigint_t a)
{
    a.sign = -a.sign;
    return a;
}

digit __sign(digit x)
{
    if (x < 0)
        return -1;
    return (digit) (x > 0);
}

void __resign(bigint_t *a)
{
    for (size_t i = 0; i < BIG_INT_SIZE; i++)
        a->content[i] *= a->sign;
}

digit __add_digit(digit *n, digit m, digit sign)
{
    *n += m;
    digit next = 0;
    if (__sign(*n) != sign && *n != 0 && sign != 0)
    {
        *n += sign * BIG_INT_BASE;
        next -= sign;
    } else if (*n >= BIG_INT_BASE)
    {
        *n -= BIG_INT_BASE;
        next += 1;
    } else if (*n <= -BIG_INT_BASE)
    {
        *n += BIG_INT_BASE;
        next -= 1;
    }

    return next;
}

error_t bi_sum(bigint_t *a, bigint_t b)
{
    if (!a)
        return NEW_ERROR("Передан пустой "
                         "указатель на большое целое", BI_ERROR_NULL_POINTER);

    __resign(a);
    __resign(&b);
    a->sign = 0;
    for (size_t i = BIG_INT_SIZE; i > 0; i--)
    {
        size_t j = i - 1;
        if (a->sign == 0 && a->content[j] != -b.content[j])
            a->sign = __sign(a->content[j] + b.content[j]);

        digit next = __add_digit(&a->content[j], b.content[j], a->sign);
        for (size_t k = i; k < BIG_INT_SIZE && next; k++)
            next = __add_digit(&a->content[k], next, a->sign);

        if (next)
            return NEW_ERROR("Переполнение большого целого", BI_ERROR_OVERFLOW);
    }

    __resign(a);
    return NEW_SUCCESS();
}

error_t bi_diff(bigint_t *a, bigint_t b)
{
    return bi_sum(a, bi_neg(b));
}

int __num_len(bigint_t a)
{
    for (size_t i = BIG_INT_SIZE; i > 0; i--)
        if (a.content[i - 1] != 0)
            return i;

    return 0;
}

void bi_rshift(bigint_t *a, int n)
{
    if (n > BIG_INT_SIZE)
        n = BIG_INT_SIZE;

    if (n < 0)
        return bi_lshift(a, -n);

    for (size_t i = 0; i < BIG_INT_SIZE - n; i++)
        a->content[i] = a->content[i + n];

    for (size_t i = BIG_INT_SIZE - n; i < BIG_INT_SIZE; i++)
        a->content[i] = 0;
}

void bi_rshift_rounded(bigint_t *a, int n)
{
    if (n > BIG_INT_SIZE)
        n = BIG_INT_SIZE;

    if (n < 0)
        return bi_lshift(a, -n);

    int next = a->content[n - 1] >= BI_ROUND_LIMIT;
    bi_rshift(a, n);
    next *= a->sign;
    bi_sum(a, bi_from_ll(next));
}

void bi_lshift(bigint_t *a, int n)
{
    if (n > BIG_INT_SIZE)
        n = BIG_INT_SIZE;

    if (n < 0)
        return bi_rshift(a, -n);

    for (size_t i = BIG_INT_SIZE; i > n; i--)
        a->content[i - 1] = a->content[i - 1 - n];

    for (size_t i = n; i > 0; i--)
        a->content[i - 1] = 0;
}

error_t bi_div(bigint_t *a, bigint_t b)
{
    if (!b.sign)
        return NEW_ERROR("Деление на нуль", BI_ERROR_ZERO_DIVISION);

    bigint_t result = bi_create();
    result.sign = a->sign * b.sign;
    a->sign = 1;
    b.sign = 1;

    if (bi_cmp(*a, b) < 0)
    {
        *a = bi_create();
        return NEW_SUCCESS();
    }

    size_t len_a = __num_len(*a);
    size_t len_b = __num_len(b);
    size_t n = len_a - len_b;
    bi_lshift(&b, n);
    for (size_t i = n + 1; i > 1; i--)
    {
        digit d = 0;
        while (bi_cmp(*a, b) >= 0)
        {
            error_t err = bi_diff(a, b);
            if (is_failure(err))
                return err;
            d++;
        }
        result.content[i - 2] = d;
        bi_rshift(&b, 1);
    }

    *a = result;

    return NEW_SUCCESS();
}

digit char_to_digit(char c)
{
    for (size_t i = 0; i < BIG_INT_BASE; i++)
        if (c == symbols[i])
            return i;

    return -1;
}

char digit_to_char(digit d)
{
    if (d >= 0 && d < BIG_INT_BASE)
        return symbols[d];

    return -1;
}

error_t bi_from_str(char *str, bigint_t *a)
{
    str = normalize_str(str);
    *a = bi_create();
    a->sign = 1;

    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            a->sign = -1;
        str++;
    }

    if (strlen(str) > BIG_INT_SIZE)
        return NEW_ERROR("Строка слишком длинная", BI_ERROR_OVERFLOW);

    size_t len = strlen(str);
    if (len == 0)
        return NEW_ERROR("Целое число пустое", BI_ERROR_INVALID);
    int s = 0;
    for (size_t i = len; i > 0; i--)
    {
        digit d = char_to_digit(str[i - 1]);
        if (d < 0)
            return NEW_ERROR("Некорректное число", BI_ERROR_INVALID);
        s += d;
        a->content[len - i] = d;
    }
    if (!s)
        a->sign = 0;

    return NEW_SUCCESS();
}
