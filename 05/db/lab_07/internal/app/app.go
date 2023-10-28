package app

import (
	"fmt"
	"gorm.io/driver/postgres"
	"gorm.io/gorm"
	"lab_07/internal/db"
	"os"
)

type App struct {
	database  *db.DB
	resolvers []resolver
}

func New(dsn string) (*App, error) {
	pureDB, err := gorm.Open(postgres.Open(dsn), &gorm.Config{})
	if err != nil {
		return nil, err
	}

	a := &App{
		database: db.New(pureDB),
	}

	a.resolvers = []resolver{
		{
			name: "Получить список всех закладок",
			f:    a.getZakladki,
		},
		{
			name: "Получить список сотрудников, подчиняющихся главному боссу",
			f:    a.getStaffWithBoss,
		},
		{
			name: "Получить топ 10 дедов",
			f:    a.getTopCostProducts,
		},
		{
			name: "Получить цену самого дорогого продукта по категориям",
			f:    a.getTopCostProducts,
		},
		{
			name: "Получить цену самого дорогого продукта по категориям, цена больше 100 000",
			f:    a.getTopCostProductsWithMinPrice,
		},
		{
			name: "Получить дополнительную инфу про зоны",
			f:    a.getZonesAdditionalInfo,
		},
		{
			name: "Добавить к дополнительной инфе есть ли там пиво",
			f:    a.addBeer,
		},
		{
			name: "Добавить новую дополнительную инфу",
			f:    a.addAdditionalInfo,
		},
		{
			name: "Вывести пары id_закладки, id_зоны",
			f:    a.getZakladkiZones,
		},
		{
			name: "Добавить закладку",
			f:    a.insertZakladka,
		},
		{
			name: "Изменить зону закладки",
			f:    a.updateZakladkaZone,
		},
		{
			name: "Удалить закладку",
			f:    a.deleteZakladka,
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
