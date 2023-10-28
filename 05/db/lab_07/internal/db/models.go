package db

import "time"

type Zakladka struct {
	ID      int
	Address string
	ZoneID  int
}

type ZakladkaZone struct {
	ID      int
	Country string
}

type Staff struct {
	ID        int
	Nickname  string
	BirthDate time.Time
	Position  string
	IsAlive   bool
	BossID    int
}

type TopCostProduct struct {
	CategoryID int
	Cost       float64
}

type AdditionalInfo struct {
	Help   string
	Bosses struct {
		Premier   string
		President string
	}
	Difficult bool
	Beer      bool `json:"beer,omitempty"`
}
