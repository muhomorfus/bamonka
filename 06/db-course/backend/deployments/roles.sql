create role employer with login password 'employer_password';
grant select,update on employee to employer;
grant select on team to employer;
grant select, insert, delete on subscription to employer;
grant select on work to employer;
grant select on department to employer;
grant select, insert on vacation to employer;

create role recruiter with login password 'recruiter_password';
grant employer to recruiter;
grant insert on employee to recruiter;
grant insert,delete on work to recruiter;

create role admin with login password 'admin_password';
grant recruiter to admin;
grant delete on employee to admin;
grant insert,update,delete on team to recruiter;
grant insert,update,delete on department to recruiter;