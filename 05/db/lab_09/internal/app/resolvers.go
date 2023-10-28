package app

import (
	"fmt"
	"lab_09/internal/models"
	"log"
	"os"
	"time"
)

const (
	staffKey       = "oldest_staff"
	getDuration    = 100 * time.Millisecond
	changeDuration = 200 * time.Millisecond
	limit          = 50
)

type resolver struct {
	name string
	f    func() error
}

func printSlice[T any](s []T) {
	for i, e := range s {
		fmt.Printf("%d) %+v\n", i+1, e)
	}
}

func (a *App) oldestStaffDBMenu() error {
	result, err := a.database.OldestStaff()
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) oldestStaffRedis() ([]models.Staff, error) {
	result, err := a.rdb.Get(staffKey)
	if err == nil {
		return result, nil
	}

	result, err = a.database.OldestStaff()
	if err != nil {
		return nil, err
	}

	if err := a.rdb.Set(staffKey, result); err != nil {
		return nil, err
	}

	return result, nil
}

func (a *App) oldestStaffRedisMenu() error {
	result, err := a.oldestStaffRedis()
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) deleteDB() error {
	if err := a.database.Delete(); err != nil {
		return err
	}

	return a.rdb.Remove(staffKey)
}

func (a *App) updateDB() error {
	if err := a.database.Update(); err != nil {
		return err
	}

	return a.rdb.Remove(staffKey)
}

func (a *App) createDB() error {
	if err := a.database.Create(); err != nil {
		return err
	}

	return a.rdb.Remove(staffKey)
}

func (a *App) bench(change func() error, n int) func() error {
	return func() error {
		tickerDB := time.NewTicker(getDuration)
		tickerRedis := time.NewTicker(getDuration)
		tickerChange := time.NewTicker(changeDuration)

		begin := time.Now()

		fRedis, err := os.Create(fmt.Sprintf("data/%02d_redis.txt", n))
		if err != nil {
			return err
		}
		defer fRedis.Close()

		fDB, err := os.Create(fmt.Sprintf("data/%02d_db.txt", n))
		if err != nil {
			return err
		}
		defer fDB.Close()

		i := 0

		for i < limit*2 {
			select {
			case <-tickerDB.C:
				i++
				log.Println("bench db")

				start := time.Now()
				a.database.OldestStaff()
				end := time.Now()

				fmt.Fprintln(fDB, end.Sub(begin).Milliseconds(), end.Sub(start).Microseconds())
			case <-tickerRedis.C:
				i++
				log.Println("bench redis")

				start := time.Now()
				a.oldestStaffRedis()
				end := time.Now()

				fmt.Fprintln(fRedis, end.Sub(begin).Milliseconds(), end.Sub(start).Microseconds())
			case <-tickerChange.C:
				if change != nil {
					log.Println("change data")
					change()
				}
			}
		}

		return nil
	}
}
