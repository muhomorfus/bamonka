#ifndef MYSTRING_H
#define MYSTRING_H

#include <stdio.h>

#define MYSTRING_SIZE 17

#define FILE_ERROR 100
#define INPUT_ERROR 101
#define ARGS_ERROR 102
#define OVERFLOW_ERROR 103
#define CONVERSION_ERROR 104

/**
 * Тип строки длиной 16 символов + терминальный нуль.
 */
typedef char mystring_t[MYSTRING_SIZE];

#endif //MYSTRING_H
