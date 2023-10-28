package db

import (
	"database/sql"
)

type DB struct {
	db *sql.DB
}

func New(db *sql.DB) *DB {
	return &DB{
		db: db,
	}
}

func (db *DB) MaxProductPrice() (float64, error) {
	var result float64

	if err := db.db.QueryRow("select max(price) from products").Scan(&result); err != nil {
		return 0, err
	}

	return result, nil
}

func (db *DB) PurchasesStat() ([]ProductPurchaseSummary, error) {
	query := `
select id, name,
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
`

	rows, err := db.db.Query(query)
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	result := make([]ProductPurchaseSummary, 0)
	for rows.Next() {
		var p ProductPurchaseSummary
		if err := rows.Scan(&p.ID, &p.Name, &p.TotalPurchased); err != nil {
			return nil, err
		}

		result = append(result, p)
	}

	return result, nil
}

func (db *DB) PurchasedProducts(date string) ([]string, error) {
	query := `
with products_numbered as (
    select p.name,
           row_number() over(partition by p.name) as rn
    from (
             purchases join products_on_storages pos on purchases.product_on_storage_id = pos.id
             ) as pps join products p on p.id = pps.product_id
    where date(pps.time) = $1
)
select name
from products_numbered
where rn = 1;
`

	rows, err := db.db.Query(query, date)
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	result := make([]string, 0)
	for rows.Next() {
		var s string
		if err := rows.Scan(&s); err != nil {
			return nil, err
		}

		result = append(result, s)
	}

	return result, nil
}

func (db *DB) Tables(schema string) ([]string, error) {
	query := `
select table_name from information_schema.tables
            where table_schema = $1
`

	rows, err := db.db.Query(query, schema)
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	result := make([]string, 0)
	for rows.Next() {
		var s string
		if err := rows.Scan(&s); err != nil {
			return nil, err
		}

		result = append(result, s)
	}

	return result, nil
}

func (db *DB) CanBuy(productID int, price float64) (bool, error) {
	var result bool

	if err := db.db.QueryRow("select can_buy($1, $2)", productID, price).Scan(&result); err != nil {
		return false, err
	}

	return result, nil
}

func (db *DB) CanBuyList(price float64) ([]string, error) {
	rows, err := db.db.Query("select name from can_buy_table($1)", price)
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	result := make([]string, 0)
	for rows.Next() {
		var s string
		if err := rows.Scan(&s); err != nil {
			return nil, err
		}

		result = append(result, s)
	}

	return result, nil
}

func (db *DB) StartAction(categoryID int, discount int) error {
	_, err := db.db.Exec("call start_action($1, $2)", discount, categoryID)
	return err
}

func (db *DB) PostgresVersion() (string, error) {
	var result string

	if err := db.db.QueryRow("select version()").Scan(&result); err != nil {
		return "", err
	}

	return result, nil
}

func (db *DB) CreateZakladkiTable() error {
	query := `
drop table if exists zakladki;
create table zakladki(
    id serial primary key,
    address text,
    zone_id int references zones on delete cascade
);
`

	_, err := db.db.Exec(query)
	return err
}

func (db *DB) InsertZakladki(z Zakladka) error {
	_, err := db.db.Exec("insert into zakladki(id, address, zone_id) values ($1, $2, $3)", z.ID, z.Address, z.ZoneID)
	return err
}

func (db *DB) BiggestStorage() (string, int, error) {
	query := `
with storages_info as (
select storage_id as id, count(*) as c from products_on_storages group by storage_id
)
select address, (select c from storages_info where id = s.id) from storages s where id = (select si.id from storages_info si where si.c = (select max(si1.c) from storages_info si1))
`

	var address string
	var n int

	if err := db.db.QueryRow(query).Scan(&address, &n); err != nil {
		return "", 0, err
	}

	return address, n, nil
}
