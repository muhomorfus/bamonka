#ifndef FLATS_DB_H
#define FLATS_DB_H

#include <stdlib.h>
#include "flat.h"

#define FILE_READ_ERROR 201
#define DELETION_ERROR 202
#define FILTER_ERROR 203

#define MAX_NUM_OF_FLATS 6000

/*
 * Ключ по площади квартиры.
 * area - площадь квартиры (значение ключа);
 * pos - позиция записи с этим ключом в исходной
 *       таблице.
 */
typedef struct
{
    double area;
    size_t pos;
} area_key_t;

/*
 * База данных с информацией о квартирах.
 * content - записи о квартирах;
 * keys - таблица ключей;
 * n - размер базы данных;
 * f - файл, в котором изначально записана база данных.
 */
typedef struct
{
    flat_t content[MAX_NUM_OF_FLATS];
    area_key_t keys[MAX_NUM_OF_FLATS];
    size_t n;
    FILE *f;
} flats_db_t;

/*
 * Считывание базы данных из файла.
 * db - база данных.
 */
error_t db_read(flats_db_t *db);

/*
 * Добавление записи о квартире в базу данных.
 * db - база данных;
 * flat - запись с информацией о квартире.
 */
error_t db_add_flat(flats_db_t *db, flat_t flat);

/*
 * Удаление записи из базы данных.
 * db - база данных;
 * key - значение ключа, по которому удаляем;
 * del - функция, определяющая, соответствует ли данная запись
 *       критерию удаления, заданному в key.
 */
error_t db_delete(flats_db_t *db, void *key, bool (*del) (flat_t, void *));

/*
 * Критерии удаления для каждого поля.
 * flat - запись, которая проверяется;
 * key - значение ключа.
 */
bool db_del_by_address(flat_t flat, void *key);
bool db_del_by_area(flat_t flat, void *key);
bool db_del_by_cost_per_square_meter(flat_t flat, void *key);
bool db_del_by_number_of_rooms(flat_t flat, void *key);
bool db_del_by_class(flat_t flat, void *key);
bool db_del_by_have_finishing(flat_t flat, void *key);
bool db_del_by_year_of_construction(flat_t flat, void *key);
bool db_del_by_number_of_owners(flat_t flat, void *key);
bool db_del_by_number_of_tenants(flat_t flat, void *key);
bool db_del_by_were_animals(flat_t flat, void *key);

/*
 * Сортировка таблицы ключей методом сортировки вставками.
 * db - база данных.
 */
void db_insertion_sort_keys(flats_db_t *db);

/*
 * Сортировка таблицы методом сортировки вставками.
 * db - база данных.
 */
void db_insertion_sort_flats(flats_db_t *db);

/*
 * Сортировка таблицы ключей методом быстрой сортировки.
 * db - база данных.
 */
void db_quick_sort_keys(flats_db_t *db);

/*
 * Сортировка таблицы методом быстрой сортировки.
 * db - база данных.
 */
void db_quick_sort_flats(flats_db_t *db);

/*
 * Фильтрация таблицы по критерию из условия.
 * db - база данных;
 * min_cost - минимальная стоимость квартиры;
 * max_cost - максимальная стоимость квартиры.
 */
error_t db_filter(flats_db_t *db, int min_cost, int max_cost);

#endif //FLATS_DB_H
