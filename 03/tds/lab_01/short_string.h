#ifndef SHORT_STRING_H
#define SHORT_STRING_H

#include <stdbool.h>

#include "error.h"

#define SHORT_STRING_SIZE 257

#ifndef SS_FILE_ERROR
#define SS_FILE_ERROR 2
#endif
#ifndef SS_INPUT_ERROR
#define SS_INPUT_ERROR 3
#endif
#ifndef SS_ARGS_ERROR
#define SS_ARGS_ERROR 4
#endif
#ifndef SS_OVERFLOW_ERROR
#define SS_OVERFLOW_ERROR 5
#endif

typedef char short_string_t[SHORT_STRING_SIZE];

/**
 * Чтение строки из файла.
 * f - файловый дескриптор;
 * str - строка, в которую вводим.
 */
error_t f_read_line(FILE *f, short_string_t str);

/**
 * Проверка строки на пустоту.
 * str - исходная строка.
 */
bool is_empty(char *str);

/**
 * Нормализация строки, удаление пробелов, возведение в один регистр.
 * str - исходная строка.
 */
char *normalize_str(char *str);

#endif //SHORT_STRING_H
