#ifndef BIGINT_H
#define BIGINT_H

#include <limits.h>

#include "config.h"
#include "error.h"

#ifndef BIG_INT_SIZE
#define BIG_INT_SIZE 64
#endif

#ifndef BIG_INT_BASE
#define BIG_INT_BASE 10
#endif

#if BIG_INT_BASE > 16
#error Base must be not bigger than 16.
#endif

#if BIG_INT_BASE <= 1
#error Base must be bigger than one.
#endif

#define BI_ERROR_NULL_POINTER 100
#define BI_ERROR_OVERFLOW 101
#define BI_ERROR_ZERO_DIVISION 102
#define BI_ERROR_INVALID 103

#define BI_ROUND_LIMIT BIG_INT_BASE / 2

typedef char digit;

/**
 * content - содержимое числа (модуль числа);
 * sign - знак числа (-1 если число отрицательное, 1
 * если положительное, иначе 0).
 */
typedef struct {
    digit content[BIG_INT_SIZE];
    digit sign;
} bigint_t;

/**
 * Создание большого целого числа.
 */
bigint_t bi_create();
/**
 * Конвертация большого целого из long long.
 * n - число, которое будем конвертировать.
 */
bigint_t bi_from_ll(long long n);

/**
 * Изменение знака числа
 * a - число, знак которого нужно изменить.
 */
bigint_t bi_neg(bigint_t a);
/**
 * Сложение двух больших целых. Результат помещается в
 * первый аргумент.
 * a - первое слагаемое;
 * b - второе слагаемое.
 */
error_t bi_sum(bigint_t *a, bigint_t b);
/**
 * Вычитание двух больших целых. Результат помещается в
 * первый аргумент.
 * a - уменьшаемое;
 * b - вычитаемое.
 */
error_t bi_diff(bigint_t *a, bigint_t b);
/**
 * Целочисленное деление больших целых. Результат помещается в
 * первый аргумент.
 * a - делимое;
 * b - делитель.
 */
error_t bi_div(bigint_t *a, bigint_t b);

/**
 * Сравнение двух больших целых.
 * a - первое число для сравнения;
 * b - второе число.
 */
int bi_cmp(bigint_t a, bigint_t b);

/**
 * Сдвиги большого целого: влево, вправо, вправо с округлением.
 * Результат помещается в первый аргумент.
 * a - сдвигаемое число;
 * n - сдвиг.
 */
void bi_lshift(bigint_t *a, int n);
void bi_rshift(bigint_t *a, int n);
void bi_rshift_rounded(bigint_t *a, int n);

/**
 * Преобразование символа в цифру.
 * c - символ.
 */
digit char_to_digit(char c);
/**
 * Преобразование цифры в число.
 * d - цифра.
 */
char digit_to_char(digit d);

/**
 * Преобразование строки в большое целое.
 * str - преобразуемая строка;
 * a - резульат.
 */
error_t bi_from_str(char *str, bigint_t *a);

#endif //BIGINT_H
