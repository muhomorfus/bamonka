#ifndef FLAT_H
#define FLAT_H

#include <stdbool.h>

#include "mystring.h"
#include "error.h"

#define MIN_NUM_OF_FIELDS 6
#define MAX_NUM_OF_FIELDS 9

/**
 * Перечисление для определения типа квартиры:
 * первичная или вторичная.
 * primary - первичная;
 * secondary - вторичная.
 */
typedef enum
{
    primary,
    secondary,
} flat_class_t;

/**
 * Информация о первичной квартире.
 * have_finishing - есть ли отделка.
 */
typedef struct
{
    bool have_finishing;
} primary_flat_info_t;

/**
 * Информация о вторичной квартире.
 * year_of_construction - год постройки;
 * number_of_owners - количество предыдущих собственников;
 * number_of_tenants - количество предыдущих жильцов;
 * were_animals - были ли животные.
 */
typedef struct
{
    int year_of_construction;
    int number_of_owners;
    int number_of_tenants;
    bool were_animals;
} secondary_flat_info_t;

/**
 * Дополнительная информация о квартире (вариативная часть).
 * primary - информация о первичной недвижимости;
 * secondary - информация о вторичной недвижимости.
 */
typedef union
{
    primary_flat_info_t primary;
    secondary_flat_info_t secondary;
} additional_info_t;

/**
 * Данные о кваритире.
 * address - адрес квартиры;
 * area - площадь;
 * number_of_rooms - количество комнат;
 * cost_per_square_meter - цена за квадратный метр;
 * class - класс квартиры: вторичная или первичная;
 * info - дополнительная информация (вариативная часть).
 */
typedef struct
{
    mystring_t address;
    double area;
    int number_of_rooms;
    int cost_per_square_meter;
    flat_class_t class;
    additional_info_t info;
} flat_t;

/**
 * Функция преобразования полей и строк.
 */
typedef error_t (*field_converter_t)(mystring_t, flat_t *);

/**
 * Набор функций-преобразователей из строковых значений
 * в поля структуры.
 * Названия функций соответствуют названиям полей,
 * которые они преобразуют.
 */
typedef struct
{
    field_converter_t address;
    field_converter_t area;
    field_converter_t number_of_rooms;
    field_converter_t cost_per_square_meter;
    field_converter_t class;
    field_converter_t have_finishing;
    field_converter_t year_of_construction;
    field_converter_t number_of_owners;
    field_converter_t number_of_tenants;
    field_converter_t were_animals;
} flat_converter_t;

/*
 * Преобразование массива строк в запись с
 * информацией о квартире.
 * strs - исходный массив строк;
 * n - количество элементов массива строк;
 * flat - запись с информацией о квартире;
 * c - преобразователь.
 */
error_t convert_to_flat(mystring_t *strs, size_t n, flat_t *flat, flat_converter_t c);

/*
 * Преобразование записи с информацией о квартире
 * в массив строк, используя все варианты
 * вариативной части.
 * strs - массив строк;
 * n - количество элементов массива строк;
 * flat - исходная запись с информацией о квартире;
 * c - преобразователь.
 */
error_t convert_all_from_flat(mystring_t *strs, size_t n, flat_t *flat, flat_converter_t c);

#endif //FLAT_H
