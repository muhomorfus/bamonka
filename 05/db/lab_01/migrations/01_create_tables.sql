drop table if exists purchases;
drop table if exists products_on_storages;
drop table if exists products;
drop table if exists categories;
drop table if exists storages;
drop table if exists consumers;
drop table if exists zones;
drop table if exists staff;
drop table if exists safety_classes;

create table safety_classes
(
    id               serial primary key,
    name             text    not null,
    num_of_guardians int     not null check (num_of_guardians > 0),
    have_air_defense boolean not null default false
);

create table staff
(
    id         serial primary key,
    nickname   text not null unique,
    birth_date date check (birth_date + interval '18 years' <= current_timestamp),
    position   text not null default 'Soldier',
    is_alive   bool not null default true,
    boss_id    int references staff (id) on delete cascade
);

create table zones
(
    id                 serial primary key,
    code_name          text    not null unique,
    country            text    not null,
    capital_address    text not null unique,
    tutor_id           int references staff (id) on delete cascade
);

create table consumers
(
    id       serial primary key,
    nickname text    not null unique,
    is_alive boolean not null default true,
    contact  text    not null,
    zone_id  int     not null references zones (id) on delete cascade
);

create table storages
(
    id              serial primary key,
    zone_id         int references zones (id) on delete cascade,
    safety_class_id int        references safety_classes (id) on delete set null,
    mentor_id       int references staff (id) on delete set null,
    address         text       not null unique
);

create table categories
(
    id          serial primary key,
    name        text not null,
    description text not null,
    parent      int references categories (id) on delete cascade,
    slug        text not null
);

create table products
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

create table products_on_storages
(
    id         serial primary key,
    product_id int references products (id) on delete cascade,
    storage_id int references storages (id) on delete cascade,
    updated_at timestamp not null,
    count      int       not null
);

create table purchases
(
    id                    serial primary key,
    product_on_storage_id int       references products_on_storages (id) on delete set null,
    consumer_id           int       references consumers (id) on delete set null,
    count                 int       not null,
    time                  timestamp not null
);

