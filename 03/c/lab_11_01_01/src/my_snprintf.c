#include <stdlib.h>
#include <stdbool.h>

#include "my_snprintf.h"

#define NUM_STR_SIZE 30

size_t my_strlen(const char *str)
{
    size_t size = 0;
    while (str[size])
        size++;
    return size;
}

size_t insert_char(char **s, char *border, char c)
{
    if (!s || !*s)
        return 1;

    if (*s == border)
        **s = '\0';

    if (*s >= border)
        return 1;

    **s = c;
    (*s)++;
    **s = '\0';

    return 1;
}

size_t insert_reversed(char **s, char *border, char *rev)
{
    if (!rev)
        return 0;

    size_t len = my_strlen(rev);
    if (!s)
        return len;

    for (char *sym = rev + len - 1; sym >= rev; sym--)
        insert_char(s, border, *sym);

    return len;
}

char digit_to_char(int digit)
{
    if (digit < 0 || digit > 16)
        return -1;

    return "0123456789abcdef"[digit];
}

size_t insert_ll(char **s, char *border, long long number, int base)
{
    if (base < 2 || base > 16)
        return 0;

    char num_str_rev[NUM_STR_SIZE] = { 0 };
    int sign = 1;
    if (number < 0)
        sign = -1;

    char *rev = &num_str_rev[0];
    while (number)
    {
        *(rev++) = digit_to_char(sign * (int) (number % base));
        number /= base;
    }
    if (sign < 0)
        *(rev++) = '-';
    if (rev == num_str_rev)
        *(rev++) = '0';

    *rev = '\0';

    return insert_reversed(s, border, num_str_rev);
}

size_t insert_ull(char **s, char *border, unsigned long long number, int base)
{
    if (base < 2 || base > 16)
        return 0;

    char num_str_rev[NUM_STR_SIZE] = { 0 };

    char *rev = &num_str_rev[0];
    while (number)
    {
        *(rev++) = digit_to_char((int) (number % base));
        number /= base;
    }
    if (rev == num_str_rev)
        *(rev++) = '0';

    *rev = '\0';

    return insert_reversed(s, border, num_str_rev);
}

size_t insert_str(char **s, char *border, char *str)
{
    if (!str)
        return 0;

    for (char *sym = str; *sym; sym++)
        insert_char(s, border, *sym);

    size_t len = my_strlen(str);
    if (!len)
    {
        insert_char(s, border, '\0');
        (*s)--;
    }

    return len;
}

int my_snprintf(char *restrict s, size_t n, const char *restrict format, ...)
{
    va_list arg;
    va_start(arg, format);
    int code = my_vsnprintf(s, n, format, arg);
    va_end(arg);
    return code;
}

int my_vsnprintf(char *restrict s, size_t n, const char *restrict format, va_list arg)
{
    size_t size = 0;
    char *current = s;
    char *border = s + n - 1;
    for (const char *fmt = format; *fmt; fmt++)
    {
        if (*fmt == '%')
        {
            fmt++;
            if (*fmt == 'c')
                size += insert_char(&current, border, (char) va_arg(arg, int));
            else if (*fmt == 's')
                size += insert_str(&current, border, va_arg(arg, char *));
            else if (*fmt == 'd' || *fmt == 'i')
                size += insert_ll(&current, border, va_arg(arg, int), 10);
            else if (*fmt == 'o')
                size += insert_ull(&current, border, va_arg(arg, unsigned int), 8);
            else if (*fmt == 'x')
                size += insert_ull(&current, border, va_arg(arg, unsigned int), 16);
            else if (*fmt == 'l')
            {
                fmt++;
                if (*fmt == 'd' || *fmt == 'i')
                    size += insert_ll(&current, border, va_arg(arg, long int), 10);
                else if (*fmt == 'o')
                    size += insert_ull(&current, border, va_arg(arg, unsigned long int), 8);
                else if (*fmt == 'x')
                    size += insert_ull(&current, border, va_arg(arg, unsigned long int), 16);
                else
                {
                    size += insert_char(&current, border, '%');
                    size += insert_char(&current, border, 'l');
                    size += insert_char(&current, border, *fmt);
                }
            }
            else if (*fmt == 'h')
            {
                fmt++;
                if (*fmt == 'd' || *fmt == 'i')
                    size += insert_ll(&current, border, (short) va_arg(arg, int), 10);
                else if (*fmt == 'o')
                    size += insert_ull(&current, border, (unsigned short) va_arg(arg, unsigned int), 8);
                else if (*fmt == 'x')
                    size += insert_ull(&current, border, (unsigned short) va_arg(arg, unsigned int), 16);
                else
                {
                    size += insert_char(&current, border, '%');
                    size += insert_char(&current, border, 'h');
                    size += insert_char(&current, border, *fmt);
                }
            }
            else
            {
                size += insert_char(&current, border, '%');
                size += insert_char(&current, border, *fmt);
            }
        }
        else
            size += insert_char(&current, border, *fmt);
    }

    return (int) size;
}
