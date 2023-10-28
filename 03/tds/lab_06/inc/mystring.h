#ifndef MYSTRING_H
#define MYSTRING_H

#include <stdio.h>

#include "error.h"

#define MYSTRING_SIZE 257

#define FILE_ERROR 100
#define INPUT_ERROR 101
#define ARGS_ERROR 102
#define OVERFLOW_ERROR 103
#define CONVERSION_ERROR 104

/**
 * Тип строки длиной 256 символов + терминальный нуль.
 */
typedef char mystring_t[MYSTRING_SIZE];

/**
 * Чтение строки из файла.
 * f - файл;
 * str - строка, в которую считываем.
 */
error_t ms_read_line(FILE *f, mystring_t str);

/**
 * Чтение целого числа из файла.
 * f - файл;
 * n - считываемое число.
 */
error_t ms_read_int_line(FILE *f, int *n);

/**
 * Чтение слова из файла.
 * f - файл;
 * str - считываемое слово.
 */
error_t ms_read_word(FILE *f, mystring_t str);

#endif //MYSTRING_H
