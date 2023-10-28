#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "flats_db.h"
#include "flat.h"
#include "error.h"
#include "mystring.h"

#define FILE_SEPARATOR ";"

#define EPS 1e-4

#define FATAL_CHECK(expr) err = expr; if (is_failure(err)) return err

error_t address_from_file(mystring_t str, flat_t *flat)
{
    if (strlen(str) >= MYSTRING_SIZE)
        return NEW_ERROR("Слишком длинная строка", OVERFLOW_ERROR);

    strcpy(flat->address, str);
    return NEW_SUCCESS();
}

error_t area_from_file(char *str, flat_t *flat)
{
    error_t err = to_double(str, &flat->area);
    if (is_failure(err))
        return err;
    if (flat->area <= 0)
        return NEW_ERROR("Некорректная площадь", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t number_of_rooms_from_file(char *str, flat_t *flat)
{
    error_t err = to_integer(str, &flat->number_of_rooms);
    if (is_failure(err))
        return err;
    if (flat->number_of_rooms <= 0)
        return NEW_ERROR("Некорректное количество комнат", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t cost_per_square_meter_from_file(char *str, flat_t *flat)
{
    error_t err = to_integer(str, &flat->cost_per_square_meter);
    if (is_failure(err))
        return err;
    if (flat->cost_per_square_meter <= 0)
        return NEW_ERROR("Некорректная цена за квадратный метр", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t class_from_file(char *str, flat_t *flat)
{
    int class;
    error_t err = to_integer(str, &class);
    if (is_failure(err))
        return err;
    if (class != 0 && class != 1)
        return NEW_ERROR("Выбран несуществующий вариант", CONVERSION_ERROR);
    flat->class = class;
    return NEW_SUCCESS();
}

error_t have_finishing_from_file(char *str, flat_t *flat)
{
    int have_finishing;
    error_t err = to_integer(str, &have_finishing);
    if (is_failure(err))
        return err;
    if (have_finishing != 0 && have_finishing != 1)
        return NEW_ERROR("Выбран несуществующий вариант", CONVERSION_ERROR);
    flat->info.primary.have_finishing = have_finishing;
    return NEW_SUCCESS();
}

error_t year_of_construction_from_file(char *str, flat_t *flat)
{
    error_t err = to_integer(str, &flat->info.secondary.year_of_construction);
    if (is_failure(err))
        return err;
    if (flat->info.secondary.year_of_construction <= 0)
        return NEW_ERROR("Некорректный год постройки", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t number_of_owners_from_file(char *str, flat_t *flat)
{
    error_t err = to_integer(str, &flat->info.secondary.number_of_owners);
    if (is_failure(err))
        return err;
    if (flat->info.secondary.number_of_owners <= 0)
        return NEW_ERROR("Некорректное количество владельцев", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t number_of_tenants_from_file(char *str, flat_t *flat)
{
    error_t err = to_integer(str, &flat->info.secondary.number_of_tenants);
    if (is_failure(err))
        return err;
    if (flat->info.secondary.number_of_tenants <= 0)
        return NEW_ERROR("Некорректное количество жильцов", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t were_animals_from_file(char *str, flat_t *flat)
{
    int were_animals;
    error_t err = to_integer(str, &were_animals);
    if (is_failure(err))
        return err;
    if (were_animals != 0 && were_animals != 1)
        return NEW_ERROR("Выбран несуществующий вариант", CONVERSION_ERROR);
    flat->info.secondary.were_animals = were_animals;

    return NEW_SUCCESS();
}

error_t f_flat_read(FILE *f, flat_t *flat)
{
    mystring_t str;
    error_t err;
    FATAL_CHECK(f_read_line(f, str));

    mystring_t fields[MAX_NUM_OF_FIELDS];
    size_t n;
    FATAL_CHECK(splitn(str, FILE_SEPARATOR, MAX_NUM_OF_FIELDS, fields, &n));
    flat_converter_t c = {
            .address = address_from_file,
            .area = area_from_file,
            .cost_per_square_meter = cost_per_square_meter_from_file,
            .number_of_rooms = number_of_rooms_from_file,
            .class = class_from_file,
            .have_finishing = have_finishing_from_file,
            .year_of_construction = year_of_construction_from_file,
            .number_of_owners = number_of_owners_from_file,
            .number_of_tenants = number_of_tenants_from_file,
            .were_animals = were_animals_from_file,
    };

    return convert_to_flat(fields, n, flat, c);
}

error_t db_add_flat(flats_db_t *db, flat_t flat)
{
    if (db->n >= MAX_NUM_OF_FLATS)
        return NEW_ERROR("Переполнение базы", OVERFLOW_ERROR);

    db->content[db->n] = flat;
    area_key_t key;
    key.area = flat.area;
    key.pos = db->n;
    db->n++;

    return NEW_SUCCESS();
}

error_t db_delete_flat_by_pos(flats_db_t *db, size_t pos)
{
    if (db->n == 0)
        return NEW_ERROR("Нельзя удалять из пустой базы", DELETION_ERROR);
    for (size_t i = pos; i < db->n - 1; i++)
        db->content[i] = db->content[i + 1];

    size_t key_pos;
    for (key_pos = 0; key_pos < db->n && db->keys[key_pos].pos != pos; key_pos++);
    for (size_t i = key_pos; i < db->n - 1; i++)
        db->keys[i] = db->keys[i + 1];

    db->n--;
    return NEW_SUCCESS();
}

bool db_del_by_address(flat_t flat, void *key)
{
    char *address = key;
    return strcmp(flat.address, address) == 0;
}

bool db_del_by_area(flat_t flat, void *key)
{
    double *area = key;
    return fabs(flat.area - *area) < EPS;
}

bool db_del_by_cost_per_square_meter(flat_t flat, void *key)
{
    int *cost_per_square_meter = key;
    return *cost_per_square_meter == flat.cost_per_square_meter;
}

bool db_del_by_number_of_rooms(flat_t flat, void *key)
{
    int *number_of_rooms = key;
    return flat.number_of_rooms == *number_of_rooms;
}

bool db_del_by_class(flat_t flat, void *key)
{
    flat_class_t *class = key;
    return *class == flat.class;
}

bool db_del_by_have_finishing(flat_t flat, void *key)
{
    bool *have_finishing = key;
    return flat.class == primary && *have_finishing == flat.info.primary.have_finishing;
}

bool db_del_by_year_of_construction(flat_t flat, void *key)
{
    int *year_of_construction = key;
    return flat.class == secondary && *year_of_construction == flat.info.secondary.year_of_construction;
}

bool db_del_by_number_of_owners(flat_t flat, void *key)
{
    int *number_of_owners = key;
    return flat.class == secondary && *number_of_owners == flat.info.secondary.number_of_owners;
}

bool db_del_by_number_of_tenants(flat_t flat, void *key)
{
    int *number_of_tenants = key;
    return flat.class == secondary && *number_of_tenants == flat.info.secondary.number_of_tenants;
}

bool db_del_by_were_animals(flat_t flat, void *key)
{
    bool *were_animals = key;
    return flat.class == secondary && *were_animals == flat.info.secondary.were_animals;
}

error_t db_delete(flats_db_t *db, void *key, bool (*del) (flat_t, void *))
{
    error_t err;

    for (size_t i = 0; i < db->n; )
        if (del(db->content[i], key))
        {
            FATAL_CHECK(db_delete_flat_by_pos(db, i));
        }
        else
            i++;

    return NEW_SUCCESS();
}

error_t db_read(flats_db_t *db)
{
    rewind(db->f);

    flat_t flat;
    error_t err;
    db->n = 0;
    while (!is_failure(f_flat_read(db->f, &flat)))
    {
        FATAL_CHECK(db_add_flat(db, flat));
    }

    if (!feof(db->f))
        return NEW_ERROR("Ошибка чтения из файла", FILE_READ_ERROR);
    return NEW_SUCCESS();
}

void db_keys_from_flats(flats_db_t *db)
{
    for (size_t i = 0; i < db->n; i++)
    {
        db->keys[i].pos = i;
        db->keys[i].area = db->content[i].area;
    }
}

void db_swap_flats(flat_t *flats, size_t i, size_t j)
{
    flat_t tmp = flats[i];
    flats[i] = flats[j];
    flats[j] = tmp;
}

void db_swap_keys(area_key_t *keys, size_t i, size_t j)
{
    area_key_t tmp = keys[i];
    keys[i] = keys[j];
    keys[j] = tmp;
}

void db_flats_from_keys(flats_db_t *db)
{
    flat_t flats[MAX_NUM_OF_FLATS] = { 0 };
    for (size_t i = 0; i < db->n; i++)
        flats[i] = db->content[db->keys[i].pos];

    for (size_t i = 0; i < db->n; i++)
        db->content[i] = flats[i];
}

void db_insertion_sort_flats(flats_db_t *db)
{
    for (size_t i = 0; i < db->n; i++)
        for (size_t j = i + 1; j > 1 && db->content[j - 1].area <
                                                db->content[j - 2].area; j--)
            db_swap_flats(db->content, j - 1, j - 2);
    db_keys_from_flats(db);
}

void db_insertion_sort_keys(flats_db_t *db)
{
    db_keys_from_flats(db);
    for (size_t i = 0; i < db->n; i++)
        for (size_t j = i + 1; j > 1 && db->keys[j - 1].area < db->keys[j - 2].area; j--)
            db_swap_keys(db->keys, j - 1, j - 2);
}

void quick_sort_flats(flat_t *flats, size_t a, size_t b)
{
    if (b <= a + 1)
        return;

    size_t last = a;
    double base = flats[a].area;
    db_swap_flats(flats, a, b - 1);
    for (size_t i = a; i < b; i++) // 9 2 7 8 3 -> 2 7 8 3 9
    {
        if (flats[i].area <= base)
            db_swap_flats(flats, i, last++);
    }

    quick_sort_flats(flats, a, last - 1);
    quick_sort_flats(flats, last, b);
}

void quick_sort_keys(area_key_t *keys, size_t a, size_t b)
{
    if (b <= a + 1)
        return;

    size_t last = a;
    double base = keys[a].area;
    db_swap_keys(keys, a, b - 1);
    for (size_t i = a; i < b; i++) // 9 2 7 8 3 -> 2 7 8 3 9
    {
        if (keys[i].area <= base)
            db_swap_keys(keys, i, last++);
    }

    quick_sort_keys(keys, a, last - 1);
    quick_sort_keys(keys, last, b);
}

void db_quick_sort_flats(flats_db_t *db)
{
    quick_sort_flats(db->content, 0, db->n);
    db_keys_from_flats(db);
}

void db_quick_sort_keys(flats_db_t *db)
{
    db_keys_from_flats(db);
    quick_sort_keys(db->keys, 0, db->n);
}

error_t db_filter(flats_db_t *db, int min_cost, int max_cost)
{
    size_t last = 0;
    for (size_t i = 0; i < db->n; i++)
    {
        double cost = db->content[i].cost_per_square_meter * db->content[i].area;

        if (cost >= min_cost && cost <= max_cost && db->content[i].class == secondary && \
            db->content[i].number_of_rooms == 2 && !db->content[i].info.secondary.were_animals)
            db_swap_flats(db->content, i, last++);
    }

    db->n = last;
    if (last == 0)
        return NEW_ERROR("Пустой результат фильтрации", FILTER_ERROR);

    db_keys_from_flats(db);
    return NEW_SUCCESS();
}


