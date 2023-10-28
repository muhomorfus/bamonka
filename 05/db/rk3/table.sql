create database rk3;

drop table if exists kpp;
drop table if exists staff;

create table staff (
    id serial primary key,
    name text,
    birth_date date,
    department text
);

create table kpp (
    staff_id int references staff(id),
    log_date date,
    log_weekday text,
    log_time time,
    log_type int
);

insert into staff(name, birth_date, department) values
('Глотов Илья Анатольевич', '2002-01-02', 'IT'),
('Кострицкий Александр Сергеевич', '1996-05-14', 'Buhgalteria'),
('Стасов Стас Стасович', '2001-02-03', 'Uborka'),
('Иванов Иван Иванович', '2002-01-02', 'IT'),
('Кекликов Ибрагим Геннадиевич', '2002-01-02', 'IT'),
('FIO1', '2004-01-02', 'Uborka'),
('FIO2', '2001-01-02', 'Buhgalteria'),
('FIO3', '1999-01-02', 'IT'),
('FIO4', '2003-01-02', 'Uborka'),
('FIO5', '1991-03-10', 'Buhgalteria'),
('FIO6', '1992-03-10', 'IT'),
('FIO7', '1993-03-10', 'Uborka'),
('FIO8', '1994-03-10', 'IT'),
('FIO9', '1995-03-10', 'Buhgalteria'),
('FIO10', '1996-03-10', 'Uborka'),
('FIO11', '1997-03-10', 'IT');

insert into kpp(staff_id, log_date, log_weekday, log_time, log_type) values
(1, '2022-12-05', 'Понедельник', '8:00', 1),
(1, '2022-12-05', 'Понедельник', '10:00', 2),
(1, '2022-12-05', 'Понедельник', '11:00', 1),
(1, '2022-12-05', 'Понедельник', '18:00', 2),
(2, '2022-12-05', 'Понедельник', '11:00', 1),
(2, '2022-12-05', 'Понедельник', '18:00', 2),
(3, '2022-12-05', 'Понедельник', '9:00', 1),
(3, '2022-12-05', 'Понедельник', '16:00', 2),

(2, '2022-12-06', 'Вторник', '7:00', 1),
(2, '2022-12-06', 'Вторник', '10:00', 2),
(3, '2022-12-06', 'Вторник', '11:00', 1),
(3, '2022-12-06', 'Вторник', '18:00', 2),
(4, '2022-12-06', 'Вторник', '11:00', 1),
(4, '2022-12-06', 'Вторник', '18:00', 2),
(5, '2022-12-06', 'Вторник', '9:00', 1),
(5, '2022-12-06', 'Вторник', '16:00', 2),

(1, '2022-12-07', 'Среда', '8:00', 1),
(1, '2022-12-07', 'Среда', '10:00', 2),
(1, '2022-12-07', 'Среда', '11:00', 1),
(1, '2022-12-07', 'Среда', '18:00', 2),
(2, '2022-12-07', 'Среда', '11:00', 1),
(2, '2022-12-07', 'Среда', '18:00', 2),
(6, '2022-12-07', 'Среда', '9:00', 1),
(6, '2022-12-07', 'Среда', '16:00', 2),

(7, '2022-12-08', 'Четверг', '8:00', 1),
(7, '2022-12-08', 'Четверг', '10:00', 2),
(1, '2022-12-08', 'Четверг', '11:00', 1),
(1, '2022-12-08', 'Четверг', '18:00', 2),
(2, '2022-12-08', 'Четверг', '11:00', 1),
(2, '2022-12-08', 'Четверг', '18:00', 2),
(3, '2022-12-08', 'Четверг', '9:00', 1),
(3, '2022-12-08', 'Четверг', '16:00', 2),

(1, '2022-12-09', 'Пятница', '8:00', 1),
(1, '2022-12-09', 'Пятница', '10:00', 2),
(2, '2022-12-09', 'Пятница', '11:00', 1),
(2, '2022-12-09', 'Пятница', '12:00', 2),
(2, '2022-12-09', 'Пятница', '12:40', 1),
(2, '2022-12-09', 'Пятница', '18:00', 2),
(5, '2022-12-09', 'Пятница', '9:00', 1),
(5, '2022-12-09', 'Пятница', '16:00', 2);

drop function if exists get_truants;
create or replace function get_truants(today date) returns table(truant_name text, truant_department text)
as $$
    begin
        return query select name as truant_name, department as truant_department from staff where staff.id not in (
             select staff_id from kpp where log_date = today and log_type = 1
        );
    end;
$$ language plpgsql;

select * from get_truants('2022-12-06');

-- 1. Найти сотрудников, опоздавших сегодня меньше чем на 5 минут.
select name from staff where id in (
select staff_id
from kpp where log_type = 1 and log_date = '2022-12-06'
group by staff_id, log_date, log_type
having min(log_time) between '9:00'::time and '9:05'::time);

-- 2. Найти сотрудников, выходивших сегодня больше чем на 10 минут
select name from staff where id in (
select staff_id from (
    select staff_id, log_time as time_start, log_type,
         lead(log_time) over (partition by staff_id, log_date order by log_time) as time_finish
    from kpp
) as kpp_exits
where time_finish is not null and time_finish - time_start > '10 minutes' and log_type = 2);

-- 3. Найти сотрудников бухгалтерии, приходящих на работу раньше 8:00
select name from staff s where s.id in (
    select k.staff_id from kpp k where log_type = 1 and log_time < '8:00'::time
) and s.department = 'Buhgalteria';