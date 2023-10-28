package app

import (
	"database/sql"
	"fmt"
	"lab_06/internal/db"
	"os"
)

type App struct {
	database  *db.DB
	resolvers []resolver
}

func New(dsn string) (*App, error) {
	pureDB, err := sql.Open("postgres", dsn)
	if err != nil {
		return nil, err
	}

	a := &App{
		database: db.New(pureDB),
	}

	a.resolvers = []resolver{
		{
			name: "Получить максимальную цену продукта",
			f:    a.maxProductPrice,
		},
		{
			name: "Получить статистику по количеству проданных товаров",
			f:    a.purchasesStats,
		},
		{
			name: "Получить названия продуктов, купленных в указанную дату",
			f:    a.purchasedProducts,
		},
		{
			name: "Получить названия таблиц в схеме \"public\"",
			f:    a.tables,
		},
		{
			name: "Можно ли купить заданный товар за заданную цену",
			f:    a.canBuy,
		},
		{
			name: "Список товаров, которые можно купить за заданную цену",
			f:    a.canBuyList,
		},
		{
			name: "Запустить акцию на категорию",
			f:    a.startAction,
		},
		{
			name: "Получить версию Postgres",
			f:    a.postgresVersion,
		},
		{
			name: "Создать таблицу с закладками",
			f:    a.createTable,
		},
		{
			name: "Добавить закладку",
			f:    a.insertToTable,
		},
		{
			name: "Найти адрес самого крупного хранилища, и количество позиций в нем",
			f:    a.biggestStorage,
		},
		{
			name: "Выход",
			f: func() error {
				os.Exit(0)
				return nil
			},
		},
	}

	return a, nil
}

func (a *App) printMenu() {
	for i, r := range a.resolvers {
		fmt.Printf("%02d - %s\n", i, r.name)
	}
}

func (a *App) Run() error {
	for {
		a.printMenu()

		fmt.Println()

		var variant int
		fmt.Print("Введите номер пункта меню: ")
		if _, err := fmt.Scan(&variant); err != nil {
			fmt.Println(err)
			continue
		}

		if variant < 0 || variant >= len(a.resolvers) {
			fmt.Printf("Ошибка: invalid menu variant\n")
			continue
		}

		if err := a.resolvers[variant].f(); err != nil {
			fmt.Printf("Ошибка: %s\n", err)
			continue
		}

		fmt.Println()
	}
}
