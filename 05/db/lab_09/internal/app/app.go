package app

import (
	"fmt"
	redisLib "github.com/go-redis/redis/v9"
	"gorm.io/driver/postgres"
	"gorm.io/gorm"
	"lab_09/internal/db"
	"lab_09/internal/redis"
	"os"
)

type App struct {
	database  *db.DB
	rdb       *redis.Redis
	resolvers []resolver
}

func New(dsn string) (*App, error) {
	pureDB, err := gorm.Open(postgres.Open(dsn), &gorm.Config{})
	if err != nil {
		return nil, err
	}

	rdb := redis.New(redisLib.NewClient(&redisLib.Options{
		Addr:     "localhost:26379",
		Password: "passw0rd",
		DB:       0,
	}))

	a := &App{
		database: db.New(pureDB),
		rdb:      rdb,
	}

	a.resolvers = []resolver{
		{
			name: "Получить список самых старых дедов (из БД)",
			f:    a.oldestStaffDBMenu,
		},
		{
			name: "Получить список самых старых дедов (из Редиски)",
			f:    a.oldestStaffRedisMenu,
		},
		{
			name: "Замерить время без всего",
			f:    a.bench(nil, 1),
		},
		{
			name: "Замерить время с обновлениями",
			f:    a.bench(a.database.Update, 2),
		},
		{
			name: "Замерить время с удалениями",
			f:    a.bench(a.database.Delete, 3),
		},
		{
			name: "Замерить время с вставками",
			f:    a.bench(a.database.Create, 4),
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
