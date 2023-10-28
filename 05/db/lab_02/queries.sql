-- 1. Инструкция SELECT, использующая предикат сравнения
-- Выбрать все покупки, в которых был куплен 1 товар
select * from purchases where count = 1;

-- 2. Инструкция SELECT, использующая предикат BETWEEN
-- Выбрать сотрудников, которые родились в 2001 году
select * from staff where birth_date between '2001-01-01' and '2001-12-31';

-- 3. Инструкция SELECT, использующая предикат LIKE
-- Выбрать товары, названия которых начинаются на "Геро"
select * from products where name like 'Геро%';

-- 4. Инструкция SELECT, использующая предикат IN с вложенным подзапросом
-- Выбрать сотрудников, которые являются менторами зон в Афганистане
select * from staff where id in
                          (select tutor_id from zones where country = 'Afghanistan');

-- 5. Инструкция SELECT, использующая предикат EXISTS с вложенным подзапросом
-- Выбрать покупателей, которые покупали товар c id = 3
select distinct * from consumers where exists(
                                               select * from purchases where consumer_id = consumers.id and exists(
                                                       select * from products_on_storages
                                                       where products_on_storages.id = purchases.product_on_storage_id
                                                         and products_on_storages.product_id = 3
                                                   )
                                           );

-- 6. Инструкция SELECT, использующая предикат сравнения с квантором
-- Выбрать товары, которые дороже всех товаров в категории "Героин"
select name, price from products
where price > all(select price from products where category_id = 12)
order by price;

-- 7. Инструкция SELECT, использующая агрегатные функции в выражениях столбцов
-- Выбрать цену самого дорогого товара для каждой категории
select categories.name, mp.max_price
from categories join (select category_id, max(price) as max_price from products group by category_id) as mp
on categories.id = mp.category_id;

-- 8. Инструкция SELECT, использующая скалярные подзапросы в выражениях столбцов
-- Выбрать суммарную стоимость проданных товаров для каждого товара
select name,
    (
        select count(p_full.cnt * p_full.price)
        from (
            products join (
                select purchases.count as cnt, product_id
                from purchases
                join products_on_storages on purchases.product_on_storage_id = products_on_storages.id
            ) as psf on products.id = psf.product_id
        ) as p_full where p_full.product_id = products.id
    ) as total_price
from products;

-- 9. Инструкция SELECT, использующая простое выражение CASE
-- Выбрать сотрудников с флагом, является ли он боевой единицей
select id, nickname,
       case position
            when 'Soldier' then true
            else false
       end as is_soldier
from staff;

-- 10. Инструкция SELECT, использующая поисковое выражение CASE
-- Выбрать товары с комментариями о цене
select id, name,
       case
           when price < 10000 then 'Почти бесплатно'
           when price < 20000 then 'Дешево'
           when price < 100000 then 'Платно'
           when price < 1000000 then 'Прилично'
           else 'Продать почки'
       end as price_comment
from products;

-- 11. Создание новой временной локальной таблицы из результирующего набора
-- Во временную таблицу помещается количество товаров в каждой категории
drop table if exists categories_stats;

select c.name, count(*)
into categories_stats
from categories c join products p on c.id = p.category_id
group by c.id;

select * from categories_stats;

-- 12. Инструкция SELECT, использующая вложенные коррелированные
-- подзапросы в качестве производных таблиц в предложении FROM
-- Выбрать id категории и цену самого дорогого продукта в этой категории
-- при максимальной цене в 100 000 и который обновлялся на складе в мае 2021 года
select ps.category_id, ps.max_price
from (
    select category_id, max(price) as max_price
    from products p
    where p.id in (select product_id from products_on_storages where updated_at between '2021-05-01' and '2021-05-31')
    group by category_id
) as ps
where ps.max_price < 100000;

-- 13. Инструкция SELECT, использующая вложенные подзапросы с уровнем
-- вложенности 3
-- Выбрать покупателей, которые покупали самый дорогой товар
select *
from consumers
where consumers.id in (
    select purchases.consumer_id
    from purchases
    where purchases.product_on_storage_id in (
        select ps.id
        from products_on_storages ps
        where ps.product_id = (
            select p.id
            from products p
            where p.price = (select max(p.price) from products p)
        )
    )
);

-- 14. Инструкция SELECT, консолидирующая данные с помощью предложения
-- GROUP BY, но без предложения HAVING
-- Для каждой категории получить максимальную цену продукта и среднюю цену продукта
select c.name, s.max_price, s.avg_price
from categories c join (
    select category_id, max(price) as max_price, avg(price) as avg_price
    from products
    group by category_id
) as s on c.id = s.category_id;

-- 15. Инструкция SELECT, консолидирующая данные с помощью предложения
-- GROUP BY и предложения HAVING
-- То же самое, но с максимальной ценой ниже средней цены за все товары
select c.name, s.max_price, s.avg_price
from categories c join (
    select category_id, max(price) as max_price, avg(price) as avg_price
    from products
    group by category_id
    having max(price) < (select avg(p.price) from products p)
) as s on c.id = s.category_id;

-- 16. Однострочная инструкция INSERT, выполняющая вставку в таблицу одной
-- строки значений
-- Добавление товара в категорию "Наркотические вещества"
insert into products(id, name, description, min_safety_class, item_name, price, slug, category_id)
values (56, 'Снюс корвус', 'По закону Архимеда, после сытного обеда, чтобы не откинуться, надо сладко вкинуться', 1, '1 упаковка (20 паков)', 1000, 'snus-corvus', 2)
on conflict do nothing;

-- 17. Многострочная инструкция INSERT, выполняющая вставку в таблицу
-- результирующего набора данных вложенного подзапроса
-- Добавить товар в 2 раза дешевле самого дешевого - сигареты "Беломор"
insert into products(id, name, description, min_safety_class, item_name, price, slug, category_id)
values (
        57,
        'Сигареты "Беломор"',
        'Убить легкие – дешево!',
        1,
        '1 пачка',
        (select min(p.price) / 2 from products p),
        'belomor',
        2
) on conflict do nothing ;

-- 18. Простая инструкция UPDATE
-- Сделать все товары дешевле на 10%
update products
set price = price * 0.9;

-- 19. Инструкция UPDATE со скалярным подзапросом в предложении SET
-- Установить среднюю цену для продукта
update products
set price = (select avg(price) from products)
where id = 52;

-- 20. Простая инструкция DELETE
-- Удалить товар по id
delete from products
where id = 52;

-- 21. Инструкция DELETE с вложенным коррелированным подзапросом в
-- предложении WHERE
-- Удалить всех клиентов из США (санкции)
delete from consumers
where zone_id in (
    select z.id from zones z where z.country = 'USA'
);

-- 22. Инструкция SELECT, использующая простое обобщенное табличное
-- выражение
-- Получить список товаров с названиями категорий
with product_categories as (
    select p.name as product, c.name as category
    from (
        products p join categories c on p.category_id = c.id
    )
)
select * from product_categories;

-- 23. Инструкция SELECT, использующая рекурсивное обобщенное табличное
-- выражение
-- Получить список топ-управляющих структурой
with recursive staff_levels as (
    select s.id as id, s.nickname as nick, 0 as level
    from staff s
    where s.boss_id is null
    union
    select s.id, s.nickname, sl.level + 1
    from staff s join staff_levels sl on s.boss_id = sl.id
)
select *
from staff_levels
where level < 3;

-- 24. Оконные функции. Использование конструкций MIN/MAX/AVG OVER()
-- Вывести название товара, id категории, цену, максимальную, среднюю
-- и минимальную цену за товар для каждой категории
select p.name, p.category_id, p.price,
max(p.price) over(partition by p.category_id),
avg(p.price) over(partition by p.category_id),
min(p.price) over(partition by p.category_id)
from products p;

-- 25. Оконные функции для устранения дублей
-- Список товаров, купленных 1 февраля 2021 года
with products_numbered as (
    select p.name,
           row_number() over(partition by p.name) as rn
    from (
             purchases join products_on_storages pos on purchases.product_on_storage_id = pos.id
             ) as pps join products p on p.id = pps.product_id
    where date(pps.time) = '2021-02-01'
)
select name
from products_numbered
where rn = 1;

-- Для конкретного заказчика вывести количество покупок, совершенных в каждой категории
with purchases_categories(category) as (
    select c.name as category
    from (
             (
                 ((purchases pch join products_on_storages pos on pch.product_on_storage_id = pos.id) pspch
                     join products p on p.id = pspch.product_id) ppch
                     join categories c on c.id = ppch.category_id
                 )
             )
    where ppch.consumer_id = 15
)
select category, count(*) from purchases_categories group by category;
