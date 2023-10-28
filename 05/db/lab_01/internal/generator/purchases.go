package generator

import (
	"fmt"
	"git.parasha.space/go/libs/faker"
	"time"
)

type purchaseKey struct {
	productOnStorageID int
	consumerID         int
	time               postgresTs
	id                 int
}

var (
	purchasesMap = make(map[purchaseKey]struct{})
)

type Purchase struct {
	ID                 int        `csv:"id"`
	ProductOnStorageID int        `csv:"product_on_storage_id"`
	ConsumerID         int        `csv:"consumer_id"`
	Count              int        `csv:"count"`
	Time               postgresTs `csv:"time"`
}

func (e Purchase) repeats() bool {
	_, ok := purchasesMap[e.key()]
	return ok
}

func (e Purchase) key() purchaseKey {
	return purchaseKey{
		productOnStorageID: e.ProductOnStorageID,
		consumerID:         e.ConsumerID,
		time:               e.Time,
		id:                 e.ID,
	}
}

func (e Purchase) add() {
	purchasesMap[e.key()] = struct{}{}
}

func (e Purchase) equal(c Purchase) bool {
	return (e.ProductOnStorageID == c.ProductOnStorageID && e.ConsumerID == c.ConsumerID && e.Time == c.Time) || e.ID == c.ID
}

func (e Purchase) equalMany(cs []Purchase) bool {
	for _, c := range cs {
		if e.equal(c) {
			return true
		}
	}

	return false
}

func genPurchase(purchases []Purchase, pss []ProductOnStorage, consumers []Consumer) Purchase {
	var p Purchase
	first := true

	for p.repeats() || first {
		first = false

		p.ID = len(purchases) + 1
		ps := faker.ArrayElement(pss)
		p.ProductOnStorageID = ps.ID
		p.Time = postgresTs(faker.Time(faker.NewTimeLimitTime(time.Date(2020, time.January, 1, 0, 0, 0, 0, time.UTC), time.Time(ps.UpdatedAt))))
		p.Count = faker.IntWithLimits(1, 20)
		p.ConsumerID = faker.ArrayElement(consumers).ID
	}

	p.add()

	return p
}

func genPurchases(n int, pss []ProductOnStorage, consumers []Consumer) ([]Purchase, error) {
	alive := make([]Consumer, 0, len(consumers))
	for _, e := range consumers {
		if e.Alive {
			alive = append(alive, e)
		}
	}

	result := make([]Purchase, 0, n)

	for i := 0; i < n; i++ {
		result = append(result, genPurchase(result, pss, alive))
		if i%10000 == 0 {
			fmt.Printf("Purchases: %d %% done\r", i*100/n)
		}
	}

	return result, nil
}
