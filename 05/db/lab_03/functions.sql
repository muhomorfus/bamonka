-- Скалярная функция
-- Можно ли купить товар за заданную цену
drop function if exists can_buy;
create or replace function can_buy(product_id int, money numeric) returns boolean
as $$
begin
    return (select p.price <= money from products p where p.id = product_id);
end
$$ language plpgsql;

select can_buy(13, 1256);

-- Подставляемая табличная функция
-- Список товаров, которые можно купить за заданную сумму
drop function if exists can_buy_table;
create or replace function can_buy_table(money numeric) returns table(id int, name text, price numeric)
as $$
begin
    return query (select
        p.id as id,
        p.name as name,
        p.price as price
    from products p where p.price <= money);
end
$$ language plpgsql;

select * from can_buy_table(5000);

-- Многооператорная табличная функция
-- Убить всех людей, связанных с указанной зоной. Вывести их ники
drop function if exists kill_zone_people;
create or replace function kill_zone_people(zone int) returns table(nick text)
as $$
begin
    update staff set is_alive = false where id in (select z.tutor_id from zones z where z.id = zone);
    update consumers set is_alive = false where zone_id = zone;

    return query (select s.nickname as nick from staff s where s.id in (select z.tutor_id from zones z where z.id = zone)
        union
    select c.nickname as nick from consumers c where c.zone_id = zone);
end
$$ language plpgsql;

select * from kill_zone_people(100);

-- Рекурсивная функция
-- Вывести список сотрудников с уровнем, не больше заданного.
-- Если уровень -1, то вывести всех сотрудников с уровнями
drop function if exists staff_with_levels;
create or replace function staff_with_levels(max_level int) returns table(id int, nick text, level int)
as $$
begin;
    with recursive staff_levels as (
        select s.id as id, s.nickname as nick, 0 as level
        from staff s
        where s.boss_id is null
        union
        select s.id, s.nickname, sl.level + 1
        from staff s join staff_levels sl on s.boss_id = sl.id
    )
    select * from staff_levels where level < max_level or level = -1;
end
$$ language sql;

select * from staff_with_levels(3);

-- Хранимая процедура без параметров или с параметрами
-- Начать акцию - снизить цену на продукты
drop procedure if exists start_action;
create or replace procedure start_action(percent int, category int)
as $$
declare discount numeric;
begin
    discount = percent / 100;
    update products set price = price * (1 - discount) where category_id = category;
end
$$ language sql;

call start_action(10, 6);

-- Рекурсивная хранимая процедура или хранимая процедура с
-- рекурсивным ОТВ
-- Повысить цену на заданное количество продуктов на 10 рублей c заданного id
drop procedure if exists increase_price;
create or replace procedure increase_price(start_id int, number int, current int)
as $$
begin
    if (current < number)
        then
        update products
        set price = price + 10
        where id = start_id + current;

        call increase_price(start_id, number, current + 1);
    end if;
end
$$ language plpgsql;

call increase_price(10, 4, 0);

-- Хранимая процедура с курсором
-- Убить всех людей, совершивших покупки в указанный период времени.
drop procedure if exists kill_buy_people;
create or replace procedure kill_buy_people(t_start date, t_end date)
as $$
declare c cursor
        for select * from purchases p
        where p.time >= t_start and p.time <= t_end;
        r record;
begin
    open c;
    loop
        fetch c into r;
        exit when not found;
        update consumers
        set is_alive = false where id = r.id;
    end loop;
    close c;
end
$$ language plpgsql;

call kill_buy_people('2021-02-01', '2021-02-05');

-- Хранимая процедура доступа к метаданным
-- Получить названия таблиц в схеме 'public', вывести на экран
drop procedure if exists get_tables;
create or replace procedure get_tables()
as $$
declare c cursor
            for select * from information_schema.tables
            where table_schema = 'public';
        r record;
begin
    open c;
    loop
        fetch c into r;
        exit when not found;
        raise notice 'table: %s', r.table_name;
    end loop;
    close c;
end
$$ language plpgsql;

call get_tables();

-- Триггер AFTER
-- Защита от взлома: в случае попытки изменения стории покупок, убить покупателей
-- чья история была затронута
drop function if exists guard_purchases;
create or replace function guard_purchases() returns trigger
as $$
begin
    raise notice 'old = %', old;
    raise notice 'new = %', new;

    update consumers c set is_alive = false where old.consumer_id = c.id;

    raise notice 'killed hacker';

    return new;
end
$$ language plpgsql;

drop trigger if exists update_purchases on purchases;

create trigger update_purchases
    after update on purchases
    for each row execute function guard_purchases();

select * from consumers c where c.is_alive = true limit 1;
select p.id
from purchases p
where p.consumer_id = 3
limit 1;

update purchases set count = 1 where id = 97;
select * from consumers c where c.id = 3 limit 1;

-- Триггер INSTEAD OF
-- При попытке добавить очень старого человека - выдать ошибку
drop view if exists staff_aged;
create view staff_aged as
select *
from staff;

drop function if exists info_old_staff;
create or replace function info_old_staff() returns trigger
as $$
begin
    if (new.birth_date + interval '70 years' <= current_timestamp)
        then
        raise exception 'too old';
    else
        insert into staff values (new.*);
    end if;

    return new;
end
$$ language plpgsql;

drop trigger if exists insert_staff on staff;

create trigger insert_staff
    instead of insert on staff_aged
    for each row execute function info_old_staff();

insert into staff_aged(id, nickname, birth_date, position, is_alive, boss_id)
values (100000, 'jeckrodk', '1900-02-03', 'Ded', true, 4);

-- Вывести все склады + классы их защиты
drop function if exists storages_classes;
create or replace function storages_classes() returns table(storage text, class text)
as $$
begin
    return query (select
        s.address as storage,
        sc.name as class
    from storages s join safety_classes sc on s.safety_class_id = sc.id);
end
$$ language plpgsql;

select * from storages_classes();
