-- 1. Из таблиц базы данных, созданной в первой лабораторной работе,
-- извлечь данные в XML (MSSQL) или JSON(Oracle, Postgres).
-- Для выгрузки в XML проверить все режимы конструкции FOR XML
copy (select row_to_json(p)
from products p)
to '/shared/lab_05/data/products.json';

-- 2. Выполнить загрузку и сохранение XML или JSON файла в таблицу.
-- Созданная таблица после всех манипуляций должна соответствовать
-- таблице базы данных, созданной в первой лабораторной работе.
drop table if exists products_from_json;
create table products_from_json
(
    id               serial primary key,
    name             text           not null,
    description      text           not null,
    min_safety_class int            references safety_classes (id) on delete set null,
    item_name        text           not null,
    price            numeric(20, 2) not null,
    slug             text           not null,
    category_id      int references categories (id) on delete cascade
);

-- Копировать из файла JSON прямо в таблицу нельзя. Надо создать табличку
-- Содержащее одно поле формата json, и ее строки уже разбивать по нужным нам
-- столбцам
drop table if exists products_json;
create table products_json(data json);

copy products_json from '/shared/lab_05/data/products.json';
select * from products_json;

-- Получение таблицы с полем JSON и соответствующими полями разбитого JSON-а
-- Разбивается по полям таблицы products_from_json, данные берутся из
-- таблицы products_json
select * from products_json, json_populate_record(null::products_from_json, data);

insert into products_from_json
select id, name, description, min_safety_class, item_name, price, slug, category_id
from products_json, json_populate_record(null::products_from_json, data);

select * from products_from_json;

-- 3. Создать таблицу, в которой будет атрибут(-ы) с типом XML или JSON,
-- или добавить атрибут с типом XML или JSON к уже существующей таблице.
-- Заполнить атрибут правдоподобными данными с помощью команд INSERT или UPDATE.
alter table zones add column additional_info jsonb;

update zones
set additional_info = case
    when country = 'Bolivia' then '{"difficult": false, "help": "@carleone"}'::jsonb
    when country = 'Morocco' then '{"difficult": false, "help": "@vasyan"}'::jsonb
    when country = 'Kiribati' then '{"difficult": false, "help": "@ivan"}'::jsonb
    when country = 'Niger' then '{"difficult": false, "help": "@nigga"}'::jsonb
    when country = 'Turkmenistan' then '{"difficult": false, "help": "@dedos"}'::jsonb
    when country = 'Uruguay' then '{"difficult": false, "help": "@dedos"}'::jsonb
    when country = 'Moldova' then '{"difficult": false, "help": "@carleone"}'::jsonb
    when country = 'Vatican City State (Holy See)' then '{"difficult": false, "help": "@carleone"}'::jsonb
    when country = 'Palau' then '{"difficult": false, "help": "@bebra"}'::jsonb
    when country = 'Thailand' then '{"difficult": false, "help": "@shelby"}'::jsonb
    when country = 'Pakistan' then '{"difficult": false, "help": "@nigga"}'::jsonb
    when country = 'Afghanistan' then '{"difficult": false, "help": "@muhomorfus"}'::jsonb
    else '{"difficult": true}'::jsonb
    end;

-- 4. Выполнить следующие действия:
-- 4.1. Извлечь XML/JSON фрагмент из XML/JSON документа
-- Получить объект по полю bosses в зонах, страна которых начинается на Y
update zones
set additional_info = '{"difficult": false, "help": "@djd", "bosses": {"president": "bidon", "premier": "nigga"}}'::jsonb
where country = 'Yugoslavia';

select id, code_name, country, additional_info->'bosses' as d
from zones
where country like 'Y%';

-- 4.2. Извлечь значения конкретных узлов или атрибутов XML/JSON
-- документа
-- Вывести список простых стран и тех, кто может с ними помочь
-- -> получения поля как объект json
-- ->> получение поля как строки
select distinct country,
                additional_info->>'help' as help
from zones
where bool(additional_info->'difficult') = false;

-- 4.3. Выполнить проверку существования узла или атрибута
-- Вывести списк зон, в которых есть поле bosses
-- ? проверка наличия поля
select id, code_name, country
from zones
where additional_info ? 'bosses';

-- 4.4. Изменить XML/JSON документ
-- Добавить Путина в Россию
-- || конкатенация объектов json
-- если при объединении ключи совпадают, то происходит перезапись
-- {"key1":1,"key2":"value"} || {"key1":2} == {"key1":2,"key2":"value"}
select additional_info || '{"bosses": {"president": "Putin"}}'::jsonb
from zones where country = 'Russia';

update zones
set additional_info = additional_info || '{"bosses": {"president": "Putin"}}'::jsonb
where country = 'Russia';

select * from zones where country = 'Russia';

-- 4.5. Разделить XML/JSON документ на несколько строк по узлам
-- Разделить информацию в JSON поле о Югославии
select * from  json_each((select distinct additional_info from zones where country = 'Yugoslavia')::json);

select array_to_json(array((select product_id from products_on_storages where storage_id = 3)));

copy (select row_to_json(storages_info) from (select
    s.address as address,
    array_to_json(
        array(
            (select p.name from products_on_storages ps join products p on ps.product_id = p.id where storage_id = s.id)
        )
    ) as products
from storages s) storages_info) to '/shared/lab_05/data/storages_info.json';
