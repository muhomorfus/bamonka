package generator

import "git.parasha.space/go/libs/faker"

type ProductOnStorage struct {
	ID        int        `csv:"id"`
	ProductID int        `csv:"product_id"`
	StorageID int        `csv:"storage_id"`
	UpdatedAt postgresTs `csv:"updated_at"`
	Count     int        `csv:"count"`
}

func (e ProductOnStorage) equal(c ProductOnStorage) bool {
	return (e.ProductID == c.ProductID && e.StorageID == c.StorageID) || e.ID == c.ID
}

func (e ProductOnStorage) equalMany(cs []ProductOnStorage) bool {
	for _, c := range cs {
		if e.equal(c) {
			return true
		}
	}

	return false
}

func genProductOnStorage(pss []ProductOnStorage, products []Product, storages [][]Storage) ProductOnStorage {
	var p ProductOnStorage
	first := true

	for p.equalMany(pss) || first {
		first = false

		p.ID = len(pss) + 1
		product := faker.ArrayElement(products)
		p.ProductID = product.ID
		p.Count = faker.IntWithLimits(1, (-8)*product.MinSafetyClassID+58)
		p.StorageID = faker.ArrayElement(faker.ArrayElement(storages[product.MinSafetyClassID-1:])).ID
		p.UpdatedAt = postgresTs(faker.Time(faker.NewTimeLimitYear(2021, 2022)))
	}

	return p
}

func genProductsOnStorages(n int, products []Product, storages []Storage, classes []SafetyClass) ([]ProductOnStorage, error) {
	probProducts := make([]Product, 0, 5*len(products))
	probProducts = append(probProducts, products[:len(products)-6]...)
	probProducts = append(probProducts, products[:len(products)-6]...)
	probProducts = append(probProducts, products[:len(products)-6]...)
	probProducts = append(probProducts, products[len(products)-6:]...)

	filteredStorages := make([][]Storage, len(classes))
	for i := range filteredStorages {
		filteredStorages[i] = make([]Storage, 0, len(storages))
	}

	for _, s := range storages {
		filteredStorages[s.SafetyClassID-1] = append(filteredStorages[s.SafetyClassID-1], s)
	}

	result := make([]ProductOnStorage, 0, n)

	for i := 0; i < n; i++ {
		result = append(result, genProductOnStorage(result, probProducts, filteredStorages))
	}

	return result, nil
}
