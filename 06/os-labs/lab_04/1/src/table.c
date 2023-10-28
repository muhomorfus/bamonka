#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "table.h"
#include "flats_db.h"
#include "flat.h"
#include "error.h"
#include "mystring.h"

#define FATAL_CHECK(expr) err = expr; if (is_failure(err)) return err

table_t main_table = {
        .fields = {
                {ADDRESS_WIDTH, "Address"},
                {AREA_WIDTH, "Area"},
                {NUMBER_OF_ROOMS_WIDTH, "Rooms"},
                {COST_PER_SQUARE_METER_WIDTH, "Cost per m^2"},
                {CLASS_WIDTH, "Class"},
                {HAVE_FINISHING_WIDTH, "Finishing"},
                {YEAR_OF_CONSTRUCTION_WIDTH, "Built"},
                {NUMBER_OF_OWNERS_WIDTH, "Owners"},
                {NUMBER_OF_TENANTS_WIDTH, "Tenants"},
                {WERE_ANIMALS_WIDTH, "Animals"},
        },
        .n = 10,
};

table_t keys_table = {
        .fields = {
                {NUM_WIDTH, "Index"},
                {NUM_WIDTH, "Position"},
                {COST_PER_SQUARE_METER_WIDTH, "Key"},
                {COST_PER_SQUARE_METER_WIDTH, "Cost per m^2"},
        },
        .n = 3,
};

error_t address_to_str(mystring_t str, flat_t *flat)
{
    strcpy(str, flat->address);
    return NEW_SUCCESS();
}

error_t area_to_str(char *str, flat_t *flat)
{
    if (sprintf(str, "%.2lf", flat->area) < 0)
        return NEW_ERROR("Ошибка при переводе в строку", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t number_of_rooms_to_str(char *str, flat_t *flat)
{
    if (sprintf(str, "%d", flat->number_of_rooms) < 0)
        return NEW_ERROR("Ошибка при переводе в строку", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t cost_per_square_meter_to_str(char *str, flat_t *flat)
{
    if (sprintf(str, "%d", flat->cost_per_square_meter) < 0)
        return NEW_ERROR("Ошибка при переводе в строку", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t class_to_str(char *str, flat_t *flat)
{
    if (flat->class == primary)
        strcpy(str, "primary");
    else if (flat->class == secondary)
        strcpy(str, "secondary");
    else
        return NEW_ERROR("Ошибка при переводе в строку", CONVERSION_ERROR);
    
    return NEW_SUCCESS();
}

error_t have_finishing_to_str(char *str, flat_t *flat)
{
    if (flat->class == primary)
    {
        if (flat->info.primary.have_finishing)
            strcpy(str, "yes");
        else
            strcpy(str, "no");
    }
    else
        strcpy(str, "-");
    return NEW_SUCCESS();
}

error_t year_of_construction_to_str(char *str, flat_t *flat)
{
    if (flat->class == secondary)
    {
        if (sprintf(str, "%d", flat->info.secondary.year_of_construction) < 0)
            return NEW_ERROR("Ошибка при переводе в строку", CONVERSION_ERROR);
    }
    else
        strcpy(str, "-");
    return NEW_SUCCESS();
}

error_t number_of_owners_to_str(char *str, flat_t *flat)
{
    if (flat->class == secondary)
    {
        if (sprintf(str, "%d", flat->info.secondary.number_of_owners) < 0)
            return NEW_ERROR("Ошибка при переводе в строку", CONVERSION_ERROR);
    }
    else
        strcpy(str, "-");
    return NEW_SUCCESS();
}

error_t number_of_tenants_to_str(char *str, flat_t *flat)
{
    if (flat->class == secondary)
    {
        if (sprintf(str, "%d", flat->info.secondary.number_of_tenants) < 0)
            return NEW_ERROR("Ошибка при переводе в строку", CONVERSION_ERROR);
    }
    else
        strcpy(str, "-");
    return NEW_SUCCESS();
}

error_t were_animals_to_str(char *str, flat_t *flat)
{
    if (flat->class == secondary)
    {
        if (flat->info.secondary.were_animals)
            strcpy(str, "yes");
        else
            strcpy(str, "no");
    }
    else
        strcpy(str, "-");
    return NEW_SUCCESS();
}

error_t print_row(table_t cfg, mystring_t *strs)
{
    error_t err;
    printf("|");
    for (size_t i = 0; i < cfg.n; i++)
    {
        FATAL_CHECK(align(strs[i], ' ', cfg.fields[i].width, false));
        printf(" %s |", strs[i]);
    }
    printf("\n");
    return NEW_SUCCESS();
}

error_t print_separator(table_t cfg)
{
    mystring_t str;
    error_t err;
    printf("+");
    for (size_t i = 0; i < cfg.n; i++)
    {
        str[0] = '\0';
        FATAL_CHECK(align(str, '-', cfg.fields[i].width, false));
        printf("-%s-+", str);
    }
    printf("\n");
    return NEW_SUCCESS();
}

error_t print_header(table_t cfg)
{
    mystring_t str;
    error_t err;
    FATAL_CHECK(print_separator(cfg));
    printf("|");
    for (size_t i = 0; i < cfg.n; i++)
    {
        strcpy(str, cfg.fields[i].name);
        FATAL_CHECK(align(str, ' ', cfg.fields[i].width, true));
        printf(" %s |", str);
    }
    printf("\n");
    FATAL_CHECK(print_separator(cfg));
    return NEW_SUCCESS();
}

error_t print_flat_row(flat_t flat)
{
    mystring_t fields[MAX_NUM_OF_FIELDS];
    error_t err;
    size_t k = 0;

    flat_converter_t c = {
            .address = address_to_str,
            .area = area_to_str,
            .cost_per_square_meter = cost_per_square_meter_to_str,
            .number_of_rooms = number_of_rooms_to_str,
            .class = class_to_str,
            .have_finishing = have_finishing_to_str,
            .year_of_construction = year_of_construction_to_str,
            .number_of_owners = number_of_owners_to_str,
            .number_of_tenants = number_of_tenants_to_str,
            .were_animals = were_animals_to_str,
    };
    
    FATAL_CHECK(convert_all_from_flat(fields, MAX_NUM_OF_FIELDS, &flat, c));

    print_row(main_table, fields);
    return NEW_SUCCESS();
}

error_t print_flats_table(flats_db_t *db)
{
    if (db->n == 0)
        return NEW_ERROR("Таблица пуста", EMPTY_TABLE_ERROR);

    error_t err;
    FATAL_CHECK(print_header(main_table));
    for (size_t i = 0; i < db->n; i++)
    {
        FATAL_CHECK(print_flat_row(db->content[i]));
    }
    FATAL_CHECK(print_separator(main_table));

    return NEW_SUCCESS();
}

error_t print_flats_table_by_keys(flats_db_t *db)
{
    if (db->n == 0)
        return NEW_ERROR("Таблица пуста", EMPTY_TABLE_ERROR);

    error_t err;
    FATAL_CHECK(print_header(main_table));
    for (size_t i = 0; i < db->n; i++)
    {
        FATAL_CHECK(print_flat_row(db->content[db->keys[i].pos]));
    }
    FATAL_CHECK(print_separator(main_table));

    return NEW_SUCCESS();
}

error_t print_key_row(area_key_t key, size_t pos)
{
    mystring_t fields[3];
    size_t k = 0;

    sprintf(fields[k++], "%zu", pos);
    sprintf(fields[k++], "%zu", key.pos);
    sprintf(fields[k], "%.1lf", key.area);

    print_row(keys_table, fields);
    return NEW_SUCCESS();
}

error_t print_keys_table(flats_db_t *db)
{
    if (db->n == 0)
        return NEW_ERROR("Таблица пуста", EMPTY_TABLE_ERROR);

    error_t err;
    FATAL_CHECK(print_header(keys_table));
    for (size_t i = 0; i < db->n; i++)
    {
        FATAL_CHECK(print_key_row(db->keys[i], i));
    }
    FATAL_CHECK(print_separator(keys_table));

    return NEW_SUCCESS();
}
