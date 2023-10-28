#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

#define CODE_SUCCESS 0

#define NEW_ERROR(text,code) new_error(text, code, __func__)
#define NEW_SUCCESS() new_success(__func__)

/**
 * text - текст ошибки;
 * code - код ошибки;
 * func - функция, в которой случилась ошибка.
 */
typedef struct 
{
    const char *text;
    int code;
    const char *func;
} error_t;

/**
 * Создание новой ошибки.
 * text - текст ошибки;
 * code - код ошибки;
 * func - функция, в которой случилась ошибка.
 */
error_t new_error(const char *text, int code, const char *func);
/**
 * Создание ошибки-маркера успеха.
 * func - функция, в которой был создан "успех".
 */
error_t new_success(const char *func);

/**
 * Проверка, отображает ли ошибка ошибочную ситуацию.
 * err - исходная ошибка.
 */
bool is_failure(error_t err);

#endif
