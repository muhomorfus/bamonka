package db

import (
	"gorm.io/gorm"
	"lab_09/internal/models"
	"math/rand"
	"time"
)

const limit = 10

type DB struct {
	db *gorm.DB
}

func New(db *gorm.DB) *DB {
	return &DB{
		db: db,
	}
}

func (db *DB) OldestStaff() ([]models.Staff, error) {
	var staff []models.Staff

	if res := db.db.Table("staff").Select("nickname, birth_date").Order("birth_date").Limit(limit).Find(&staff); res.Error != nil {
		return nil, res.Error
	}

	return staff, nil
}

func (db *DB) Delete() error {
	var count int64
	db.db.Table("staff").Count(&count)

	if res := db.db.Table("staff").Where("id = ?", count).Delete(&models.Staff{}); res.Error != nil {
		return res.Error
	}

	return nil
}

func (db *DB) Update() error {
	var count int64
	db.db.Table("staff").Count(&count)

	if res := db.db.Table("staff").Where("id = ?", rand.Int63n(count)).Update("birth_date", time.Date(1970+rand.Intn(20), time.January, 1, 0, 0, 0, 0, time.UTC)); res.Error != nil {
		return res.Error
	}

	return nil
}

func (db *DB) Create() error {
	var count int64
	db.db.Table("staff").Count(&count)

	rand.Seed(time.Now().UnixNano())

	if res := db.db.Table("staff").Create(&models.Staff{
		ID:        int(count) + 1,
		Nickname:  randString(50),
		BirthDate: time.Date(1970, time.January, 1, 0, 0, 0, 0, time.UTC),
		Position:  "Soldier",
		IsAlive:   true,
		BossID:    4,
	}); res.Error != nil {
		return res.Error
	}

	return nil
}

const letterBytes = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

func randString(n int) string {
	b := make([]byte, n)
	for i := range b {
		b[i] = letterBytes[rand.Intn(len(letterBytes))]
	}
	return string(b)
}
