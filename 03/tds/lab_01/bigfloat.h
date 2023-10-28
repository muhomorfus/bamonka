#ifndef BIGFLOAT_H
#define BIGFLOAT_H

#include "config.h"
#include "bigint.h"
#include "error.h"

#ifndef BIG_FLOAT_SIZE
#define BIG_FLOAT_SIZE 32
#endif

#ifndef BIG_FLOAT_MAX_EXPONENT
#define BIG_FLOAT_MAX_EXPONENT 9999
#endif

#ifndef BIG_FLOAT_MIN_EXPONENT
#define BIG_FLOAT_MIN_EXPONENT -9999
#endif

#ifndef BIG_FLOAT_EXP_SYMBOL
#define BIG_FLOAT_EXP_SYMBOL 'e'
#endif

#define BF_ERROR_OVERFLOW 200
#define BF_ERROR_INVALID_STR 201
#define BF_ERROR_NULL_POINTER 202
#define BF_ERROR_EQUAL_POINTERS 203

#define BF_COMPUTE_SHIFT BIG_FLOAT_SIZE + 1

/**
 * significand - мантисса числа;
 * exponent - порядок.
 */
typedef struct {
    bigint_t significand;
    int exponent;
} bigfloat_t;

/**
 * Преобразование строки в большое число с
 * плавающей точкой.
 * str - исходная строка;
 * a - результат преобразования.
 */
error_t bf_from_str(char *str, bigfloat_t *a);
/**
 * Преобразование большого числа с плавающей
 * точкой в строку.
 * str - результат преобразования;
 * a - преобразуемое число.
 */
error_t bf_to_str(char *str, bigfloat_t a);
/**
 * Преобразование большого целое в большое число
 * с плавающей точкой.
 * n - преобразуемое число;
 * a - результат преобразования.
 */
error_t bf_from_bigint(bigint_t n, bigfloat_t *a);

/**
 * Деление больших чисел с плавающей точкой.
 * a - делимое;
 * b - делитель.
 */
error_t bf_div(bigfloat_t *a, bigfloat_t b);

#endif //BIGFLOAT_H
