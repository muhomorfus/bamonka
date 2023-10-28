package db

type ProductPurchaseSummary struct {
	ID             int
	Name           string
	TotalPurchased int
}

type Zakladka struct {
	ID      int
	Address string
	ZoneID  int
}
