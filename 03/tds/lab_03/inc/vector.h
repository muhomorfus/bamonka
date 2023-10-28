#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdio.h>

#define V_ALLOC_ERROR 120
#define V_ARGS_ERROR 121
#define V_OUT_OF_RANGE_ERROR 122
#define V_POINTER_ERROR 123
#define V_READ_ERROR 124
#define V_PRINT_ERROR 125
#define V_ARITHMETIC_ERROR 126

/*
 * Верхняя граница значения случайного элемента.
 */
#define V_RANDOM_ELEMENT_BORDER 10

/*
 * Вектор-столбец.
 * ptr - указатель на элементы;
 * allocated - размер выделенной памяти;
 * len - длина вектора.
 */
typedef struct v_vector_t
{
    double *ptr;
    size_t allocated;
    size_t len;
} v_vector_t;

/*
 * Выделена ли память под вектор.
 * v - исходный вектор.
 */
bool v_allocated(v_vector_t *v);

/*
 * Инициализирован ли вектор.
 * v - исходный вектор.
 */
bool v_initialized(v_vector_t *v);

/*
 * Получение пустого неаллоцированного "объекта" вектора.
 */
v_vector_t v_v();

/*
 * Выделение памяти под вектор.
 * v - исходный вектор;
 * len - длина вектора.
 */
error_t v_allocate(v_vector_t *v, size_t len);

/*
 * Инициализация вектора.
 * v - исходный вектор;
 * len - длина вектора.
 */
error_t v_init(v_vector_t *v, size_t len);

/*
 * Освобождение вектора.
 */
void v_free(v_vector_t *v);


/*
 * Считывание длины вектора из файла.
 * len - длина;
 * f - файл.
 */
error_t v_read_len(size_t *len, FILE *f);

/*
 * Считывание вектора.
 * v - исходный вектор;
 * f - файл.
 */
error_t v_read(v_vector_t *v, FILE *f);

/*
 * Инициализация и считывание вектора с размерами.
 * v - исходный вектор;
 * f - файл.
 */
error_t v_from_file(v_vector_t *v, FILE *f);

/*
 * Запись вектора в файл.
 * v - исходный вектор;
 * f - файл.
 */
error_t v_write(v_vector_t *v, FILE *f);

/*
 * Случайной заполнение вектора.
 * v - исходный вектор.
 */
error_t v_generate(v_vector_t *v);

#endif //VECTOR_H
