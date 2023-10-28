create extension if not exists plpython3u;

-- 1. Скалярная функция CLR.
-- Средняя цена на товар заданной категории
drop function if exists products_average_price;
create or replace function products_average_price(cid int)
returns numeric
language plpython3u
as $$
    query = f"""
    select avg(price) as a
    from products
    where category_id = {cid}
    """
    res = plpy.execute(query)
    if res:
        return res[0]['a']
    return 0
$$;

select products_average_price(2);

-- 2. Пользовательская агрегатная функция CLR.
-- Средняя цена на товар заданной категории
drop function if exists products_average_price_py;
create or replace function products_average_price_py(cid int)
returns numeric
language plpython3u
as $$
    query = f"""
    select price
    from products
    where category_id = {cid}
    """
    res = plpy.execute(query)
    l = len(res)
    s = 0
    for x in res:
        s += x['price']
    s = s / l
    return s
$$;

select products_average_price_py(2);

-- 3. Табличная функция CLR.
-- Получить табличку из ника и id для клиентов с именами cодержащими подстроку
drop function if exists search_consumers;
create or replace function search_consumers(substr text) returns table(cid int, cname text)
language plpython3u
as $$
    query = f"""
    select distinct c.id, c.nickname
    from consumers as c
    where c.nickname like ('%{substr}%')
    """
    res = plpy.execute(query)
    if res:
        for client in res:
            yield (client['id'], client['nickname'])
$$;

select * from search_consumers('rol');

-- 4. Хранимая процедура CLR.
-- Поднять цену на заданную величину на продукты заданной категории
drop procedure if exists up_price;
create or replace procedure up_price(cid int, delta numeric)
language plpython3u
as $$
    query = f"""
    update products set price = price + {delta} where category_id = {cid}
    """
    res = plpy.execute(query)
$$;

call up_price(3, 20);

-- 5. Триггер CLR.
-- Не вставлять сотрудников с ником root.
create or replace function staff_check()
returns trigger
language plpython3u
as $$
    query = f"""
    insert into staff values ('{TD['new']['id']}', '{TD['new']['nickname']}', '{TD['new']['birth_date']}', '{TD['new']['position']}'
    , '{TD['new']['is_alive']}', '{TD['new']['boss_id']}')
    """

    nickname = TD["new"]["nickname"]

    if (nickname == "root"):
        plpy.fatal("forbidden root")
    else:
        plpy.execute(query)
$$;

drop view if exists staff_view;
create view staff_view as
select *
from staff;

drop trigger if exists insert_staff on staff;

create trigger insert_staff
    instead of insert on staff_view
    for each row
    execute function staff_check();

insert into staff_view(id, nickname, birth_date, position, is_alive, boss_id)
values (353454635, 'hellosmcjlnfkjv dkгfvfv', '2002-04-29', 'Soldier', true, 3)
on conflict do nothing;

insert into staff_view(id, nickname, birth_date, position, is_alive, boss_id)
values (1224324354, 'root', '2002-04-29', 'Soldier', true, 3)
on conflict do nothing;

-- 6. Тип данных CLR.
-- Тип данных под cоответствие товара и категории
drop type if exists product_category cascade;
create type product_category as
(
    product text,
    category text
);

drop table pcp;
create table pcp (
    pc product_category,
    price numeric
);

insert into pcp values
(('Героин', 'Heroine'), 50),
(('Героин Азербайджанский', 'Heroine'), 50),
(('Героин Афган', 'Heroine'), 50),
(('Героин Пивной', 'Heroine Beer'), 50),
(('балтика 9', 'Beer'), 50);

select (pc).category from pcp;

drop function if exists get_products_categories;
create or replace function get_products_categories()
    returns setof text
    language plpython3u
as $$
	query = """
	select * from pcp
	"""
	res = plpy.execute(query)
	return ([row['pc']['product'] for row in res])
$$;

select * from get_products_categories();