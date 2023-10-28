#include <stdio.h>
#include <string.h>

#include "ui.h"
#include "error.h"
#include "mystring.h"
#include "flats_db.h"
#include "flat.h"
#include "table.h"
#include "benchmark.h"

#define FATAL_CHECK(expr) err = expr; if (is_failure(err)) return err
#define MENU_CHECK(expr) err = expr; if (is_failure(err)) {printf("%d: %s (%s).\n", err.code, err.text, err.func); continue;} true

void print_main_menu()
{
    printf("+-------------------------------------------- Главное меню ---------------------------------------------+\n");
    printf("| 01 - вывод таблицы c информацией обо всех квартирах;                                                  |\n");
    printf("| 02 - добавление записи в конец таблицы;                                                               |\n");
    printf("| 03 - удаление записи по полю;                                                                         |\n");
    printf("| 04 - вывод информации о вторичных двухкомнатных квартирах в указанном ценовом диапазоне без животных; |\n");
    printf("| 05 - сортировка таблицы ключей методом быстрой сортировки;                                            |\n");
    printf("| 06 - сортировка таблицы ключей методом сортировки вставками;                                          |\n");
    printf("| 07 - сортировка таблицы методом быстрой сортировки;                                                   |\n");
    printf("| 08 - сортировка таблицы методом сортировки вставками;                                                 |\n");
    printf("| 09 - сортировка таблицы методом быстрой сортировки с помощью таблицы ключей;                          |\n");
    printf("| 10 - сортировка таблицы методом сортировки вставками с помощью таблицы ключей;                        |\n");
    printf("| 11 - возврат таблицы к исходному состоянию;                                                           |\n");
    printf("| 12 - сравнение эффективности сортировки таблицы с помощью таблицы ключей и без;                       |\n");
//    printf("| 13 - сравнение эффективности сортировки таблицы с помощью различных методов сортировки;               |\n");
    printf("| 00 - выход.                                                                                           |\n");
    printf("+-------------------------------------------------------------------------------------------------------+\n");
}

error_t address_from_input(mystring_t str, flat_t *flat)
{
    if (strlen(str) >= MYSTRING_SIZE)
        return NEW_ERROR("Слишком длинная строка", OVERFLOW_ERROR);

    strcpy(flat->address, str);
    return NEW_SUCCESS();
}

error_t area_from_input(char *str, flat_t *flat)
{
    error_t err = to_double(str, &flat->area);
    if (is_failure(err))
        return err;

    if (flat->area <= 0)
        return NEW_ERROR("Некорректная площадь", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t number_of_rooms_from_input(char *str, flat_t *flat)
{
    error_t err = to_integer(str, &flat->number_of_rooms);
    if (is_failure(err))
        return err;
    if (flat->number_of_rooms <= 0)
        return NEW_ERROR("Некорректное количество комнат", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t cost_per_square_meter_from_input(char *str, flat_t *flat)
{
    error_t err = to_integer(str, &flat->cost_per_square_meter);
    if (is_failure(err))
        return err;
    if (flat->cost_per_square_meter <= 0)
        return NEW_ERROR("Некорректная цена за квадратный метр", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t class_from_input(char *str, flat_t *flat)
{
    int class;
    error_t err = to_integer(str, &class);
    if (is_failure(err))
        return err;
    if (class != 1 && class != 2)
        return NEW_ERROR("Выбран несуществующий вариант", INPUT_CONV_ERROR);
    flat->class = class - 1;
    return NEW_SUCCESS();
}

error_t have_finishing_from_input(char *str, flat_t *flat)
{
    int have_finishing;
    error_t err;
    FATAL_CHECK(to_integer(str, &have_finishing));
    if (have_finishing != 1 && have_finishing != 2)
        return NEW_ERROR("Выбран несуществующий вариант", INPUT_CONV_ERROR);
    flat->info.primary.have_finishing = !(have_finishing - 1);
    return NEW_SUCCESS();
}

error_t year_of_construction_from_input(char *str, flat_t *flat)
{
    error_t err = to_integer(str, &flat->info.secondary.year_of_construction);
    if (is_failure(err))
        return err;
    if (flat->info.secondary.year_of_construction <= 0)
        return NEW_ERROR("Некорректный год постройки", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t number_of_owners_from_input(char *str, flat_t *flat)
{
    error_t err = to_integer(str, &flat->info.secondary.number_of_owners);
    if (is_failure(err))
        return err;
    if (flat->info.secondary.number_of_owners <= 0)
        return NEW_ERROR("Некорректное количество владельцев", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t number_of_tenants_from_input(char *str, flat_t *flat)
{
    error_t err = to_integer(str, &flat->info.secondary.number_of_tenants);
    if (is_failure(err))
        return err;
    if (flat->info.secondary.number_of_tenants <= 0)
        return NEW_ERROR("Некорректное количество жильцов", CONVERSION_ERROR);

    return NEW_SUCCESS();
}

error_t were_animals_from_input(char *str, flat_t *flat)
{
    int were_animals;
    error_t err;
    FATAL_CHECK(to_integer(str, &were_animals));
    if (were_animals != 1 && were_animals != 2)
        return NEW_ERROR("Выбран несуществующий вариант", INPUT_CONV_ERROR);
    flat->info.secondary.were_animals = !(were_animals - 1);

    return NEW_SUCCESS();
}

error_t input_address(char *str)
{
    printf("Адрес дома: ");
    return f_read_line(stdin, str);
}

error_t input_area(char *str)
{
    printf("Площадь квартиры: ");
    return f_read_line(stdin, str);
}

error_t input_number_of_rooms(char *str)
{
    printf("Количество комнат: ");
    return f_read_line(stdin, str);
}

error_t input_cost_per_square_meter(char *str)
{
    printf("Цена за квадратный метр: ");
    return f_read_line(stdin, str);
}

error_t input_class(char *str)
{
    printf("+-- Класс квартиры --+\n");
    printf("| 1 - первичная;     |\n");
    printf("| 2 - вторичная.     |\n");
    printf("+--------------------+\n");
    printf("Выберите класс: ");
    return f_read_line(stdin, str);
}

error_t input_have_finishing(char *str)
{
    printf("+-- Есть ли отделка? --+\n");
    printf("| 1 - да;              |\n");
    printf("| 2 - нет.             |\n");
    printf("+----------------------+\n");
    printf("Выберите: ");
    return f_read_line(stdin, str);
}

error_t input_year_of_construction(char *str)
{
    printf("Год постройки: ");
    return f_read_line(stdin, str);
}

error_t input_number_of_owners(char *str)
{
    printf("Количество предыдущих владельцев: ");
    return f_read_line(stdin, str);
}

error_t input_number_of_tenants(char *str)
{
    printf("Количество текущих жильцов: ");
    return f_read_line(stdin, str);
}

error_t input_were_animals(char *str)
{
    printf("+-- Жили ли раньше в квартире животные? --+\n");
    printf("| 1 - да;                                 |\n");
    printf("| 2 - нет.                                |\n");
    printf("+-----------------------------------------+\n");
    printf("Выберите: ");
    return f_read_line(stdin, str);
}

error_t process_add_record(flats_db_t *db)
{
    error_t err;
    mystring_t fields[MAX_NUM_OF_FIELDS];
    size_t k = 0;
    FATAL_CHECK(input_address(fields[k++]));
    FATAL_CHECK(input_area(fields[k++]));
    FATAL_CHECK(input_number_of_rooms(fields[k++]));
    FATAL_CHECK(input_cost_per_square_meter(fields[k++]));
    FATAL_CHECK(input_class(fields[k++]));
    if (!strcmp(fields[k - 1], "1"))
    {
        FATAL_CHECK(input_have_finishing(fields[k++]));
    }
    else if (!strcmp(fields[k - 1], "2"))
    {
        FATAL_CHECK(input_year_of_construction(fields[k++]));
        FATAL_CHECK(input_number_of_owners(fields[k++]));
        FATAL_CHECK(input_number_of_tenants(fields[k++]));
        FATAL_CHECK(input_were_animals(fields[k++]));
    }
    else
        return NEW_ERROR("Заданного варианта не существует", INPUT_CONV_ERROR);

    flat_converter_t c = {
            .address = address_from_input,
            .area = area_from_input,
            .cost_per_square_meter = cost_per_square_meter_from_input,
            .number_of_rooms = number_of_rooms_from_input,
            .class = class_from_input,
            .have_finishing = have_finishing_from_input,
            .year_of_construction = year_of_construction_from_input,
            .number_of_owners = number_of_owners_from_input,
            .number_of_tenants = number_of_tenants_from_input,
            .were_animals = were_animals_from_input,
    };

    flat_t flat;
    FATAL_CHECK(convert_to_flat(fields, k, &flat, c));
    return db_add_flat(db, flat);
}

error_t process_deletion(flats_db_t *db)
{
    mystring_t str;
    error_t err;
    int field;

    printf("+----------------- Поля для удаления -----------------+\n");
    printf("| 01 - адрес;                                         |\n");
    printf("| 02 - площадь;                                       |\n");
    printf("| 03 - количество комнат;                             |\n");
    printf("| 04 - цена за квадратный метр;                       |\n");
    printf("| 05 - класс;                                         |\n");
    printf("| 06 - наличие отделки;                               |\n");
    printf("| 07 - год постройки;                                 |\n");
    printf("| 08 - количество владельцев;                         |\n");
    printf("| 09 - количество жильцов;                            |\n");
    printf("| 10 - проживание животных;                           |\n");
    printf("+-----------------------------------------------------+\n");
    printf("Выберите поле, по которому будет удаление: ");
    FATAL_CHECK(f_read_line(stdin, str));
    FATAL_CHECK(to_integer(str, &field));

    printf("Укажите значение поля для удаления.\n");
    flat_t flat;
    void *key;
    bool (*func) (flat_t, void *);

    if (field == 1)
    {
        FATAL_CHECK(input_address(str));
        FATAL_CHECK(address_from_input(str, &flat));
        key = flat.address;
        func = db_del_by_address;
    }
    else if (field == 2)
    {
        FATAL_CHECK(input_area(str));
        FATAL_CHECK(area_from_input(str, &flat));
        key = &flat.area;
        func = db_del_by_area;
    }
    else if (field == 3)
    {
        FATAL_CHECK(input_number_of_rooms(str));
        FATAL_CHECK(number_of_rooms_from_input(str, &flat));
        key = &flat.number_of_rooms;
        func = db_del_by_number_of_rooms;
    }
    else if (field == 4)
    {
        FATAL_CHECK(input_cost_per_square_meter(str));
        FATAL_CHECK(cost_per_square_meter_from_input(str, &flat));
        key = &flat.cost_per_square_meter;
        func = db_del_by_cost_per_square_meter;
    }
    else if (field == 5)
    {
        FATAL_CHECK(input_class(str));
        FATAL_CHECK(class_from_input(str, &flat));
        key = &flat.class;
        func = db_del_by_class;
    }
    else if (field == 6)
    {
        FATAL_CHECK(input_have_finishing(str));
        FATAL_CHECK(have_finishing_from_input(str, &flat));
        key = &flat.info.primary.have_finishing;
        func = db_del_by_have_finishing;
    }
    else if (field == 7)
    {
        FATAL_CHECK(input_year_of_construction(str));
        FATAL_CHECK(year_of_construction_from_input(str, &flat));
        key = &flat.info.secondary.year_of_construction;
        func = db_del_by_year_of_construction;
    }
    else if (field == 8)
    {
        FATAL_CHECK(input_number_of_owners(str));
        FATAL_CHECK(number_of_owners_from_input(str, &flat));
        key = &flat.info.secondary.number_of_owners;
        func = db_del_by_number_of_owners;
    }
    else if (field == 9)
    {
        FATAL_CHECK(input_number_of_tenants(str));
        FATAL_CHECK(number_of_tenants_from_input(str, &flat));
        key = &flat.info.secondary.number_of_tenants;
        func = db_del_by_number_of_tenants;
    }
    else if (field == 10)
    {
        FATAL_CHECK(input_were_animals(str));
        FATAL_CHECK(were_animals_from_input(str, &flat));
        key = &flat.info.secondary.were_animals;
        func = db_del_by_were_animals;
    }
    else
        return NEW_ERROR("Введен неправильный вариант поля", INPUT_CONV_ERROR);

    FATAL_CHECK(db_delete(db, key, func));
    return NEW_SUCCESS();
}

error_t process_filtering(flats_db_t *db)
{
    int min_cost, max_cost;
    error_t err;
    mystring_t str;

    printf("Введите нижнюю границу цены: ");
    FATAL_CHECK(f_read_line(stdin, str));
    FATAL_CHECK(to_integer(str, &min_cost));

    printf("Введите верхнюю границу цены: ");
    FATAL_CHECK(f_read_line(stdin, str));
    FATAL_CHECK(to_integer(str, &max_cost));

    flats_db_t db_filtered = *db;
    FATAL_CHECK(db_filter(&db_filtered, min_cost, max_cost));
    return print_flats_table(&db_filtered);
}

error_t process_main(flats_db_t *db)
{
    mystring_t choice_str;
    int choice = -1;
    error_t err;

    while (choice != 0)
    {
        printf("\n");
        print_main_menu();

        printf("Выберите пункт меню: ");
        MENU_CHECK(f_read_line(stdin, choice_str));
        MENU_CHECK(to_integer(choice_str, &choice));

        if (choice == 1)
        {
            MENU_CHECK(print_flats_table(db));
        }
        else if (choice == 2)
        {
            MENU_CHECK(process_add_record(db));
            printf("Запись успешно добавлена.\n");
        }
        else if (choice == 3)
        {
            size_t start_size = db->n;
            MENU_CHECK(process_deletion(db));
            printf("Удалено %zu записей.\n", start_size - db->n);
        }
        else if (choice == 4)
        {
            MENU_CHECK(process_filtering(db));
        }
        else if (choice == 5)
        {
            db_quick_sort_keys(db);
            MENU_CHECK(print_keys_table(db));
        }
        else if (choice == 6)
        {
            db_insertion_sort_keys(db);
            MENU_CHECK(print_keys_table(db));
        }
        else if (choice == 7)
        {
            db_quick_sort_flats(db);
            MENU_CHECK(print_flats_table(db));
        }
        else if (choice == 8)
        {
            db_insertion_sort_flats(db);
            MENU_CHECK(print_flats_table(db));
        }
        else if (choice == 9)
        {
            db_quick_sort_keys(db);
            MENU_CHECK(print_flats_table_by_keys(db));
        }
        else if (choice == 10)
        {
            db_insertion_sort_keys(db);
            MENU_CHECK(print_flats_table_by_keys(db));
        }
        else if (choice == 11)
        {
            MENU_CHECK(db_read(db));
        }
        else if (choice == 12)
        {
            long long time_isort = benchmark(db_insertion_sort_flats, db);
            long long time_isort_by_keys = benchmark(db_insertion_sort_keys, db);
            long long time_qsort = benchmark(db_quick_sort_flats, db);
            long long time_qsort_by_keys = benchmark(db_quick_sort_keys, db);

            printf("Сортировка таблицы без таблицы ключей\n");
            printf("+----------------------+---------------+\n");
            printf("|   Метод сортировки   |     Время     |\n");
            printf("+----------------------+---------------+\n");
            printf("| Сортировка вставками | %13lld |\n", time_isort);
            printf("| Быстрая сортировка   | %13lld |\n", time_qsort);
            printf("+----------------------+---------------+\n");
            printf("\n");

            printf("Сортировка таблицы с помощью таблицы ключей\n");
            printf("+----------------------+---------------+\n");
            printf("|   Метод сортировки   |     Время     |\n");
            printf("+----------------------+---------------+\n");
            printf("| Сортировка вставками | %13lld |\n",
                   time_isort_by_keys);
            printf("| Быстрая сортировка   | %13lld |\n",
                   time_qsort_by_keys);
            printf("+----------------------+---------------+\n");
            printf("\n");

            mystring_t qsort_efficiency_str;
            mystring_t isort_efficiency_str;
            if (time_qsort_by_keys == 0)
                sprintf(qsort_efficiency_str, "                - ");
            else
                sprintf(qsort_efficiency_str, "%15.0lf%%", ((double) time_qsort / (double) time_qsort_by_keys - 1) * 100);


            if (time_isort_by_keys == 0)
                sprintf(isort_efficiency_str, "                - ");
            else
                sprintf(isort_efficiency_str, "%15.0lf%%", ((double) time_isort / (double) time_isort_by_keys - 1) * 100);

            printf("Относительное преимущество таблицы ключей\n");
            printf("+----------------------+------------------+\n");
            printf("|   Метод сортировки   |   Преимущество   |\n");
            printf("+----------------------+------------------+\n");
            printf("| Сортировка вставками | %s |\n",
                   isort_efficiency_str);
            printf("| Быстрая сортировка   | %s |\n",
                   qsort_efficiency_str);
            printf("+----------------------+------------------+\n");

            size_t flats_size = sizeof(db->content[0]) * db->n + sizeof(db->n) + sizeof(db->f);
            size_t keys_size = sizeof(db->keys[0]) * db->n;
            size_t all_size = flats_size + keys_size;
            double diff_percent = ((double) all_size / (double) flats_size - 1) * 100;

            printf("Размер исходной таблицы данных: %zu Б\n", flats_size);
            printf("Размер таблицы ключей: %zu Б\n", keys_size);
            printf("Размер всей таблицы: %zu Б\n", all_size);
            printf("Разница размеров с таблицей ключей и без: %.0lf%%\n", diff_percent);
        }
//        else if (choice == 13)
//        {
//            long long time_insertion_sort = benchmark(db_insertion_sort_flats,
//                                                      db);
//            long long time_insertion_sort_keys = benchmark(
//                    db_insertion_sort_keys, db);
//            long long time_quick_sort = benchmark(db_quick_sort_flats, db);
//            long long time_quick_sort_keys = benchmark(db_quick_sort_keys, db);
//
//            printf("Сортировка таблиц\n");
//            printf("+----------------------+---------------+\n");
//            printf("|   Метод сортировки   |     Время     |\n");
//            printf("+----------------------+---------------+\n");
//            printf("| Сортировка вставками | %13lld |\n", time_insertion_sort);
//            printf("| Быстрая сортировка   | %13lld |\n", time_quick_sort);
//            printf("+----------------------+---------------+\n");
//            printf("\n");
//
//            printf("Сортировка таблицы ключей\n");
//            printf("+----------------------+---------------+\n");
//            printf("|   Метод сортировки   |     Время     |\n");
//            printf("+----------------------+---------------+\n");
//            printf("| Сортировка вставками | %13lld |\n",
//                   time_insertion_sort_keys);
//            printf("| Быстрая сортировка   | %13lld |\n", time_quick_sort_keys);
//            printf("+----------------------+---------------+\n");
//        }
        else if (choice == 0)
            printf("Выход из основного цикла меню...\n");
        else
        {
            MENU_CHECK(NEW_ERROR("Неверный пункт меню", INPUT_ERROR));
        }
    }

    return NEW_SUCCESS();
}