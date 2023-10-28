package models

import "time"

type Staff struct {
	ID        int
	Nickname  string
	BirthDate time.Time
	Position  string
	IsAlive   bool
	BossID    int
}
