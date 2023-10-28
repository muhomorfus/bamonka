drop function if exists check_consumer_alive cascade;
drop function if exists check_enough_safe cascade;
drop function if exists check_product_count cascade;

create function check_enough_safe(storage_id int, product_id int) returns boolean
as $$
select (select safety_class_id from storages s where s.id = storage_id) >= (select min_safety_class from products p where p.id = product_id)
$$ language sql;

create function check_product_count(ps_id int, n int, t timestamp) returns boolean
as $$
with c as (
    select count, updated_at from products_on_storages ps where ps.id = ps_id
)
select count >= n or updated_at >= t from c;
$$ language sql;

create function check_consumer_alive(consumer_id int) returns boolean
as $$
select is_alive from consumers where id = consumer_id;
$$ language sql;

alter table products_on_storages
add constraint storage_enough_safe check (check_enough_safe(storage_id, product_id));

alter table purchases
add constraint product_count_valid check (check_product_count(product_on_storage_id, count, time));

alter table purchases
add constraint consumer_is_alive check (check_consumer_alive(consumer_id));