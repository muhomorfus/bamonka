package app

import (
	"fmt"
	"lab_07/internal/db"
)

var additionalZonesInfo []string

type resolver struct {
	name string
	f    func() error
}

func printSlice[T any](s []T) {
	for i, e := range s {
		fmt.Printf("%d) %+v\n", i+1, e)
	}
}

func (a *App) getZakladki() error {
	result, err := a.database.GetZakladki()
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) getStaffWithBoss() error {
	result, err := a.database.GetStaffWithBoss(1)
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) getTop10OldestStaff() error {
	result, err := a.database.GetStaffByBirthday(10)
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) getTopCostProducts() error {
	result, err := a.database.GetTopCostProducts()
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) getTopCostProductsWithMinPrice() error {
	result, err := a.database.GetTopCostProductsWithMinPrice(100000)
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) getZonesAdditionalInfo() error {
	result, err := a.database.GetAdditionalInfo()
	if err != nil {
		return err
	}

	printSlice(result)

	additionalZonesInfo = result

	return nil
}

func (a *App) addBeer() error {
	result, err := a.database.UpdateAdditionalInfo(additionalZonesInfo)
	if err != nil {
		return err
	}

	printSlice(result)

	additionalZonesInfo = result

	return nil
}

func (a *App) addAdditionalInfo() error {
	result, err := a.database.AddAdditionalInfo(additionalZonesInfo, db.AdditionalInfo{
		Help: "@govno",
		Bosses: struct {
			Premier   string
			President string
		}{
			Premier:   "Putin",
			President: "Putin",
		},
		Difficult: true,
		Beer:      true,
	})
	if err != nil {
		return err
	}

	printSlice(result)

	additionalZonesInfo = result

	return nil
}

func (a *App) getZakladkiStruct() error {
	result, err := a.database.GetZakladkiStruct()
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) getZakladkiZones() error {
	result, err := a.database.GetZakladkiZones()
	if err != nil {
		return err
	}

	printSlice(result)

	return nil
}

func (a *App) insertZakladka() error {
	var zID int

	fmt.Print("Введите id героиновой закладки: ")

	if _, err := fmt.Scan(&zID); err != nil {
		return err
	}

	var zone int

	fmt.Print("Введите id зоны: ")

	if _, err := fmt.Scan(&zone); err != nil {
		return err
	}

	var address string

	fmt.Print("Введите адрес герыча: ")

	if _, err := fmt.Scan(&address); err != nil {
		return err
	}

	z := db.Zakladka{
		ID:      zID,
		Address: address,
		ZoneID:  zone,
	}

	return a.database.InsertZakladka(z)
}

func (a *App) updateZakladkaZone() error {
	var zID int

	fmt.Print("Введите id героиновой закладки: ")

	if _, err := fmt.Scan(&zID); err != nil {
		return err
	}

	var zone int

	fmt.Print("Введите id зоны: ")

	if _, err := fmt.Scan(&zone); err != nil {
		return err
	}

	return a.database.UpdateZakladkaZone(zID, zone)
}

func (a *App) deleteZakladka() error {
	var zID int

	fmt.Print("Введите id героиновой закладки: ")

	if _, err := fmt.Scan(&zID); err != nil {
		return err
	}

	return a.database.DeleteZakladka(zID)
}

//func (a *App) purchasesStats() error {
//	result, err := a.database.PurchasesStat()
//	if err != nil {
//		return err
//	}
//
//	printSlice(result)
//
//	return nil
//}
//
//func (a *App) purchasedProducts() error {
//	fmt.Print("Введите дату в формате \"2006-01-02\": ")
//
//	var date string
//
//	if _, err := fmt.Scan(&date); err != nil {
//		return err
//	}
//
//	result, err := a.database.PurchasedProducts(date)
//	if err != nil {
//		return err
//	}
//
//	printSlice(result)
//
//	return nil
//}
//
//func (a *App) tables() error {
//	result, err := a.database.Tables("public")
//	if err != nil {
//		return err
//	}
//
//	printSlice(result)
//
//	return nil
//}
//
//func (a *App) getStaffWithBoss() error {
//	var productID int
//	var price float64
//
//	fmt.Print("Введите id продукта: ")
//
//	if _, err := fmt.Scan(&productID); err != nil {
//		return err
//	}
//
//	fmt.Print("Введите цену: ")
//
//	if _, err := fmt.Scan(&price); err != nil {
//		return err
//	}
//
//	result, err := a.database.CanBuy(productID, price)
//	if err != nil {
//		return err
//	}
//
//	fmt.Println(result)
//
//	return nil
//}
//
//func (a *App) canBuyList() error {
//	var price float64
//
//	fmt.Print("Введите цену: ")
//
//	if _, err := fmt.Scan(&price); err != nil {
//		return err
//	}
//
//	result, err := a.database.CanBuyList(price)
//	if err != nil {
//		return err
//	}
//
//	printSlice(result)
//
//	return nil
//}
//
//func (a *App) startAction() error {
//	var categoryID int
//	var discount int
//
//	fmt.Print("Введите id категории: ")
//
//	if _, err := fmt.Scan(&categoryID); err != nil {
//		return err
//	}
//
//	fmt.Print("Введите процент скидки: ")
//
//	if _, err := fmt.Scan(&discount); err != nil {
//		return err
//	}
//
//	err := a.database.StartAction(categoryID, discount)
//	if err != nil {
//		return err
//	}
//
//	fmt.Println("Акция успешно начата.")
//
//	return nil
//}
//
//func (a *App) postgresVersion() error {
//	result, err := a.database.PostgresVersion()
//	if err != nil {
//		return err
//	}
//
//	fmt.Println(result)
//
//	return nil
//}
//
//func (a *App) createTable() error {
//	err := a.database.CreateZakladkiTable()
//	if err != nil {
//		return err
//	}
//
//	fmt.Println("Таблица успешно создана.")
//
//	return nil
//}
//
//func (a *App) insertToTable() error {
//	var id int
//	var zoneID int
//
//	fmt.Print("Введите id: ")
//
//	if _, err := fmt.Scan(&id); err != nil {
//		return err
//	}
//
//	fmt.Print("Введите id зоны: ")
//
//	if _, err := fmt.Scan(&zoneID); err != nil {
//		return err
//	}
//
//	fmt.Print("Введите адрес: ")
//
//	address, err := bufio.NewReader(os.Stdin).ReadString('\n')
//
//	if err != nil {
//		return err
//	}
//
//	err = a.database.InsertZakladki(db.Zakladka{
//		ID:      id,
//		Address: address,
//		ZoneID:  zoneID,
//	})
//
//	if err != nil {
//		return err
//	}
//
//	fmt.Println("Запись успешно вставлена.")
//
//	return nil
//}
//
//func (a *App) biggestStorage() error {
//	address, n, err := a.database.BiggestStorage()
//	if err != nil {
//		return err
//	}
//
//	fmt.Println(address)
//	fmt.Println(n)
//
//	return nil
//}
