#ifndef HT_H
#define HT_H

#include <stdlib.h>

#include "mystring.h"

#define HT_ARGS_ERROR 600
#define HT_ALLOC_ERROR 601
#define HT_ALREADY_EXIST_ERROR 602
#define HT_EMPTY_ERROR 603
#define HT_NOT_FOUND_ERROR 604
#define HT_OVERFLOW_ERROR 604
#define HT_READ_ERROR 605

/**
 * Список-элемент хеш-таблицы.
 * key - ключ (слово);
 * next - указатель на следующий элемент списка.
 */
typedef struct ht_element_t ht_element_t;
struct ht_element_t
{
    mystring_t key;
    ht_element_t *next;
};

/**
 * Хеш-таблица.
 * table - элементы таблицы (списки);
 * size - емкость хеш-таблицы;
 * hash - функция хеширования.
 */
typedef struct ht_t
{
    ht_element_t **table;
    size_t size;
    size_t (*hash) (mystring_t, size_t);
} ht_t;

/**
 * Создание новой хеш-таблицы.
 * size - емкость хеш-табицы;
 * hash - функция хеширования.
 */
ht_t *ht_new(size_t size, size_t (*hash) (mystring_t, size_t));

/**
 * Освобождение хеш-таблицы.
 * ht - исходная хеш-таблица.
 */
void ht_free(ht_t **ht);

/**
 * Вставка слова в хеш-таблицу.
 * ht - исходная хеш-таблица;
 * key - вставляемый ключ (слово).
 */
error_t ht_insert(ht_t **ht, mystring_t key);

/**
 * Удаление элемента из хеш-таблицы.
 * ht - исходная хеш-таблица;
 * key - удаляемый ключ (слово).
 */
error_t ht_remove(ht_t **ht, mystring_t key, size_t *n_cmp);

/**
 * Поиск элемента в хеш-таблице.
 * ht - исходная хеш-таблица;
 * key - искомый ключ;
 * found - найденный элемент.
 */
error_t ht_search(ht_t **ht, mystring_t key, ht_element_t **found, size_t *n_cmp);

/**
 * Вывод хеш-таблицы.
 * ht - исходная хеш-таблица.
 */
error_t ht_print(ht_t **ht);

/**
 * Ввод хеш-таблицы из файла.
 * ht - исходная хеш-таблица;
 * f - файл.
 */
error_t ht_from_file(ht_t **ht, FILE *f);

/**
 * Аддитивная хеш-функция.
 * str - преобразуемая строка;
 * size - емкость хеш-таблицы.
 */
size_t ht_additive_method(mystring_t str, size_t size);

#endif //HT_H
