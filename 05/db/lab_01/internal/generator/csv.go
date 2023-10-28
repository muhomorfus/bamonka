package generator

import (
	"github.com/gocarina/gocsv"
	"os"
)

func writeToCSV(filename string, data interface{}) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	err = gocsv.MarshalFile(data, file)
	if err != nil {
		return err
	}

	return nil
}

func GenerateToCSV() error {
	categories, err := genCategories()
	if err != nil {
		return err
	}

	if err := writeToCSV("data/categories.csv", categories); err != nil {
		return err
	}

	classes, err := genSafetyClasses()
	if err != nil {
		return err
	}

	if err := writeToCSV("data/safety_classes.csv", classes); err != nil {
		return err
	}

	products, err := genProducts()
	if err != nil {
		return err
	}

	if err := writeToCSV("data/products.csv", products); err != nil {
		return err
	}

	staff, err := genStaff(60000)
	if err != nil {
		return err
	}

	if err := writeToCSV("data/staff.csv", staff); err != nil {
		return err
	}

	zones, err := genZones(1000, staff)
	if err != nil {
		return err
	}

	if err := writeToCSV("data/zones.csv", zones); err != nil {
		return err
	}

	storages, err := genStorages(4567, zones, staff, classes)
	if err != nil {
		return err
	}

	if err := writeToCSV("data/storages.csv", storages); err != nil {
		return err
	}

	productsOnStorages, err := genProductsOnStorages(50000, products, storages, classes)
	if err != nil {
		return err
	}

	if err := writeToCSV("data/products_on_storages.csv", productsOnStorages); err != nil {
		return err
	}

	consumers, err := genConsumers(10000, zones)
	if err != nil {
		return err
	}

	if err := writeToCSV("data/consumers.csv", consumers); err != nil {
		return err
	}

	purchases, err := genPurchases(500000, productsOnStorages, consumers)
	if err != nil {
		return err
	}

	if err := writeToCSV("data/purchases.csv", purchases); err != nil {
		return err
	}

	return nil
}
