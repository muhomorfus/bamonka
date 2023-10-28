package main

import (
	"database/sql"
	"fmt"
	_ "github.com/lib/pq"
	"gorm.io/driver/postgres"
	"gorm.io/gorm"
	"os"
)

type PureDB struct {
	db *sql.DB
}

func NewPureDB(dsn string) (*PureDB, error) {
	db, err := sql.Open("postgres", dsn)
	if err != nil {
		return nil, err
	}

	return &PureDB{
		db: db,
	}, nil
}

func (db *PureDB) Task1(date string) error {
	query := `
select s.name from staff s where id in (
select staff_id
from kpp where log_type = 1 and log_date = $1
group by staff_id, log_date, log_type
having min(log_time) between '9:00'::time and '9:05'::time);
`

	rows, err := db.db.Query(query, date)
	if err != nil {
		return err
	}
	defer rows.Close()

	for rows.Next() {
		var name string
		if err := rows.Scan(&name); err != nil {
			return err
		}

		fmt.Println(name)
	}

	return nil
}

func (db *PureDB) Task2() error {
	query := `
select s.name from staff s where id in (
select staff_id from (
    select staff_id, log_time as time_start, log_type,
         lead(log_time) over (partition by staff_id, log_date order by log_time) as time_finish
    from kpp
) as kpp_exits
where time_finish is not null and time_finish - time_start > '10 minutes' and log_type = 2);
`

	rows, err := db.db.Query(query)
	if err != nil {
		return err
	}
	defer rows.Close()

	for rows.Next() {
		var name string
		if err := rows.Scan(&name); err != nil {
			return err
		}

		fmt.Println(name)
	}

	return nil
}

func (db *PureDB) Task3() error {
	query := `
select s.name from staff s where s.id in (
    select k.staff_id from kpp k where log_type = 1 and log_time < '8:00'::time
) and s.department = 'Buhgalteria';
`

	rows, err := db.db.Query(query)
	if err != nil {
		return err
	}
	defer rows.Close()

	for rows.Next() {
		var name string
		if err := rows.Scan(&name); err != nil {
			return err
		}

		fmt.Println(name)
	}

	return nil
}

type GormDB struct {
	db *gorm.DB
}

func NewGormDB(dsn string) (*GormDB, error) {
	db, err := gorm.Open(postgres.Open(dsn), &gorm.Config{})
	if err != nil {
		return nil, err
	}

	return &GormDB{
		db: db,
	}, nil
}

func (db *GormDB) Task1(date string) error {
	var ids []int
	err := db.db.Table("kpp").
		Select("staff_id").
		Where("log_type = 1 and log_date = ?", date).
		Group("staff_id").
		Group("log_date").
		Group("log_type").
		Having("min(log_time) between '9:00'::time and '9:05'::time").
		Find(&ids).
		Error

	if err != nil {
		return err
	}

	var names []string
	err = db.db.Table("staff").
		Select("name").
		Where("id in ?", ids).
		Find(&names).
		Error

	if err != nil {
		return err
	}

	for _, e := range names {
		fmt.Println(e)
	}

	return nil
}

func (db *GormDB) Task2() error {
	var ids []int
	err := db.db.Table("(?) as kpp_exits",
		db.db.Table("kpp").Select("staff_id, log_time as time_start, log_type, lead(log_time) over (partition by staff_id, log_date order by log_time) as time_finish")).
		Select("staff_id").
		Where("time_finish is not null and time_finish - time_start > '10 minutes' and log_type = 2").
		Find(&ids).
		Error

	if err != nil {
		return err
	}

	var names []string
	err = db.db.Table("staff").
		Select("name").
		Where("id in ?", ids).
		Find(&names).
		Error

	if err != nil {
		return err
	}

	for _, e := range names {
		fmt.Println(e)
	}

	return nil
}

func (db *GormDB) Task3() error {
	var ids []int
	err := db.db.
		Table("kpp").
		Select("staff_id").
		Where("log_type = 1 and log_time < '8:00'::time").
		Find(&ids).
		Error

	if err != nil {
		return err
	}

	var names []string
	err = db.db.Table("staff").
		Select("name").
		Where("id in ?", ids).
		Find(&names).
		Error

	if err != nil {
		return err
	}

	for _, e := range names {
		fmt.Println(e)
	}

	return nil
}

func main() {
	dsn := "host=localhost user=user password=password dbname=rk3 sslmode=disable"

	pureDB, err := NewPureDB(dsn)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	gormDB, err := NewGormDB(dsn)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	var date string
	date = "2022-12-06"

	fmt.Print("Введите дату > ")
	if _, err := fmt.Scan(&date); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	fmt.Println("SQL:")

	fmt.Println("Task 1")
	if err := pureDB.Task1(date); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	fmt.Println("Task 2")
	if err := pureDB.Task2(); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	fmt.Println("Task 3")
	if err := pureDB.Task3(); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	fmt.Println()

	fmt.Println("GORM:")

	fmt.Println("Task 1")
	if err := gormDB.Task1(date); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	fmt.Println("Task 2")
	if err := gormDB.Task2(); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	fmt.Println("Task 3")
	if err := gormDB.Task3(); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}
