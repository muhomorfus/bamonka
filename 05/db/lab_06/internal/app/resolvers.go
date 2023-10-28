package app

import (
	"bufio"
	"fmt"
	"lab_06/internal/db"
	"os"
)

type resolver struct {
	name string
	f    func() error
}

func printSlice[T any](s []T) {
	for _, e := range s {
		fmt.Printf("%+v\n", e)
	}
}

func (a *App) maxProductPrice() error {
	result, err := a.database.MaxProductPrice()
	if err != nil {
		return err
	}

	fmt.Println(result)

	return nil
}

func (a *App) purchasesStats() error {
	result, err := a.database.PurchasesStat()
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) purchasedProducts() error {
	fmt.Print("Введите дату в формате \"2006-01-02\": ")

	var date string

	if _, err := fmt.Scan(&date); err != nil {
		return err
	}

	result, err := a.database.PurchasedProducts(date)
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) tables() error {
	result, err := a.database.Tables("public")
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) canBuy() error {
	var productID int
	var price float64

	fmt.Print("Введите id продукта: ")

	if _, err := fmt.Scan(&productID); err != nil {
		return err
	}

	fmt.Print("Введите цену: ")

	if _, err := fmt.Scan(&price); err != nil {
		return err
	}

	result, err := a.database.CanBuy(productID, price)
	if err != nil {
		return err
	}

	fmt.Println(result)

	return nil
}

func (a *App) canBuyList() error {
	var price float64

	fmt.Print("Введите цену: ")

	if _, err := fmt.Scan(&price); err != nil {
		return err
	}

	result, err := a.database.CanBuyList(price)
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) startAction() error {
	var categoryID int
	var discount int

	fmt.Print("Введите id категории: ")

	if _, err := fmt.Scan(&categoryID); err != nil {
		return err
	}

	fmt.Print("Введите процент скидки: ")

	if _, err := fmt.Scan(&discount); err != nil {
		return err
	}

	err := a.database.StartAction(categoryID, discount)
	if err != nil {
		return err
	}

	fmt.Println("Акция успешно начата.")

	return nil
}

func (a *App) postgresVersion() error {
	result, err := a.database.PostgresVersion()
	if err != nil {
		return err
	}

	fmt.Println(result)

	return nil
}

func (a *App) createTable() error {
	err := a.database.CreateZakladkiTable()
	if err != nil {
		return err
	}

	fmt.Println("Таблица успешно создана.")

	return nil
}

func (a *App) insertToTable() error {
	var id int
	var zoneID int

	fmt.Print("Введите id: ")

	if _, err := fmt.Scan(&id); err != nil {
		return err
	}

	fmt.Print("Введите id зоны: ")

	if _, err := fmt.Scan(&zoneID); err != nil {
		return err
	}

	fmt.Print("Введите адрес: ")

	address, err := bufio.NewReader(os.Stdin).ReadString('\n')

	if err != nil {
		return err
	}

	err = a.database.InsertZakladki(db.Zakladka{
		ID:      id,
		Address: address,
		ZoneID:  zoneID,
	})

	if err != nil {
		return err
	}

	fmt.Println("Запись успешно вставлена.")

	return nil
}

func (a *App) biggestStorage() error {
	address, n, err := a.database.BiggestStorage()
	if err != nil {
		return err
	}

	fmt.Println(address)
	fmt.Println(n)

	return nil
}
