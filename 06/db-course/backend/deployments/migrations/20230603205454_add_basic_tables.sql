-- +goose Up
-- +goose StatementBegin
create extension if not exists "uuid-ossp";

create table team (
    uuid uuid primary key,
    name text not null,
    description text not null
);

create table department (
    uuid uuid primary key,
    parent_uuid uuid,
    name text not null,
    description text not null
);

create type level_t as enum (
    'user',
    'hr',
    'admin'
);

create table employee (
    uuid uuid primary key,
    name text not null,
    nickname text not null unique,
    department_uuid uuid not null references department(uuid),
    position text not null,
    email text not null,
    phone text not null,
    social json not null,
    boss_uuid uuid,
    description text not null,
    created_at timestamp not null default current_timestamp,
    deleted_at timestamp,
    level level_t not null default 'user',
    password text not null,
    photo text not null
);

create table subscription (
    uuid uuid primary key,
    author_uuid uuid not null references employee(uuid),
    subscriber_uuid uuid not null references employee(uuid)
);

create table vacation (
    uuid uuid primary key,
    employee_uuid uuid not null references employee(uuid),
    description text not null,
    start timestamp not null,
    "end" timestamp not null
);

create table work (
    uuid uuid primary key,
    employee_uuid uuid not null references employee(uuid) on delete cascade,
    team_uuid uuid not null references team(uuid) on delete cascade,
    "role" text not null,
    created_at timestamp not null default current_timestamp
);

create or replace function department_breadcrumbs(id uuid) returns table(uuid uuid, name text)
as $$
begin
    return query with recursive departments as (
        select d.uuid, d.name, d.parent_uuid
        from department d
        where d.uuid = id
        union
        select d.uuid, d.name, d.parent_uuid
        from department d join departments ds on d.uuid = ds.parent_uuid
    )
    select d.uuid, d.name from departments d;
end
$$ language plpgsql;
-- +goose StatementEnd

-- +goose Down
-- +goose StatementBegin
drop function department_breadcrumbs;

drop table work;
drop table vacation;
drop table subscription;
drop table employee;
drop type level_t;
drop table department;
drop table team;
-- +goose StatementEnd
