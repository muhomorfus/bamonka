#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>

#include "mystring.h"
#include "flats_db.h"

#define ADDRESS_WIDTH 20
#define AREA_WIDTH 7
#define NUMBER_OF_ROOMS_WIDTH 7
#define COST_PER_SQUARE_METER_WIDTH 14
#define CLASS_WIDTH 10
#define HAVE_FINISHING_WIDTH 10
#define YEAR_OF_CONSTRUCTION_WIDTH 7
#define NUMBER_OF_OWNERS_WIDTH 7
#define NUMBER_OF_TENANTS_WIDTH 7
#define WERE_ANIMALS_WIDTH 7

#define NUM_WIDTH 10

#define EMPTY_TABLE_ERROR 401

/*
 * Данные о поле таблицы.
 * width - ширина поля;
 * name - название поля.
 */
typedef struct
{
    int width;
    mystring_t name;
} table_field_t;

/*
 * Информация о всех полях таблицы.
 * fields - данные о полях таблицы;
 * n - количество полей таблицы.
 */
typedef struct
{
    table_field_t fields[MAX_NUM_OF_FLATS];
    size_t n;
} table_t;

/*
 * Печать таблицы квартир.
 * db - база данных о квартирах.
 */
error_t print_flats_table(flats_db_t *db);

/*
 * Печать таблицы квартир по таблице ключей.
 * db - база данных о квартирах.
 */
error_t print_flats_table_by_keys(flats_db_t *db);

/*
 * Печать таблицы ключей.
 * db - база данных о квартирах.
 */
error_t print_keys_table(flats_db_t *db);

#endif //TABLE_H
