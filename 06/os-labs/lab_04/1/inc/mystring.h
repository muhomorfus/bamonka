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

/*
 * Тип строки длиной 256 символов + терминальный нуль.
 */
typedef char mystring_t[MYSTRING_SIZE];

/*
 * Чтение строки из файла.
 * f - файл;
 * str - строка, в которую считываем.
 */
error_t f_read_line(FILE *f, mystring_t str);

/*
 * Преобразование строки в число с плавающей точкой.
 * str - исходная строка;
 * res - результат.
 */
error_t to_double(char *str, double *res);

/*
 * Преобразование строки в целое число.
 * str - исходная строка;
 * res - результат.
 */
error_t to_integer(char *str, int *res);

/*
 * Удаление пробельных символов с двух сторон строки.
 * str - исходная строка.
 */
void trim_spaces(char *str);

/*
 * Разбиение строки по разделителю.
 * str - исходная строка;
 * seps - разделители;
 * words - количество максимальных слов разбиения;
 * res - результат;
 * n - количество слов в результате разбиения.
 */
error_t splitn(char *str, char *seps, size_t words, mystring_t *res, size_t *n);

/*
 * Выравнивание строки.
 * str - исходная строка;
 * sym - символ, которым заполняется свободное пространство;
 * len - до какой длины выравнивать;
 * center - центрировать ли строку.
 */
error_t align(mystring_t str, char sym, size_t len, bool center);

#endif //MYSTRING_H
