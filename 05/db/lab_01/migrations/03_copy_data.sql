truncate table purchases cascade;
truncate table consumers cascade;
truncate table products_on_storages cascade;
truncate table storages cascade;
truncate table zones cascade;
truncate table staff cascade;
truncate table safety_classes cascade;
truncate table products cascade;
truncate table categories cascade;

copy categories FROM '/shared/lab_01/data/categories.csv' DELIMITER ',' CSV HEADER;

copy safety_classes FROM '/shared/lab_01/data/safety_classes.csv' DELIMITER ',' CSV HEADER;

copy products FROM '/shared/lab_01/data/products.csv' DELIMITER ',' CSV HEADER;

copy staff FROM '/shared/lab_01/data/staff.csv' DELIMITER ',' CSV HEADER;

copy zones FROM '/shared/lab_01/data/zones.csv' DELIMITER ',' CSV HEADER;

copy storages FROM '/shared/lab_01/data/storages.csv' DELIMITER ',' CSV HEADER;

copy products_on_storages FROM '/shared/lab_01/data/products_on_storages.csv' DELIMITER ',' CSV HEADER;

copy consumers FROM '/shared/lab_01/data/consumers.csv' DELIMITER ',' CSV HEADER;

copy purchases FROM '/shared/lab_01/data/purchases.csv' DELIMITER ',' CSV HEADER;