package db

import (
	"encoding/json"
	"gorm.io/gorm"
)

type DB struct {
	db *gorm.DB
}

func New(db *gorm.DB) *DB {
	return &DB{
		db: db,
	}
}

func (db *DB) GetZakladki() ([]map[string]interface{}, error) {
	var z []map[string]interface{}

	if res := db.db.Table("zakladki").Find(&z); res.Error != nil {
		return nil, res.Error
	}

	return z, nil
}

func (db *DB) GetStaffWithBoss(bossID int) ([]map[string]interface{}, error) {
	var s []map[string]interface{}

	if res := db.db.Table("staff").Find(&s, "boss_id = ?", bossID); res.Error != nil {
		return nil, res.Error
	}

	return s, nil
}

func (db *DB) GetStaffByBirthday(limit int) ([]map[string]interface{}, error) {
	var s []map[string]interface{}

	if res := db.db.Table("staff").Order("birth_date").Limit(limit).Find(&s); res.Error != nil {
		return nil, res.Error
	}

	return s, nil
}

func (db *DB) GetTopCostProducts() ([]map[string]interface{}, error) {
	var p []map[string]interface{}

	if res := db.db.Table("products").Select("category_id, max(price) as cost").Group("category_id").Find(&p); res.Error != nil {
		return nil, res.Error
	}

	return p, nil
}

func (db *DB) GetTopCostProductsWithMinPrice(minPrice float64) ([]map[string]interface{}, error) {
	var p []map[string]interface{}

	if res := db.db.Table("products").Select("category_id, max(price) as cost").Group("category_id").Having("max(price) > ?", minPrice).Find(&p); res.Error != nil {
		return nil, res.Error
	}

	return p, nil
}

func (db *DB) GetAdditionalInfo() ([]string, error) {
	var s []string

	if res := db.db.Table("zones").Distinct().Select("additional_info").Find(&s); res.Error != nil {
		return nil, res.Error
	}

	return s, nil
}

func (db *DB) UpdateAdditionalInfo(info []string) ([]string, error) {
	for i, e := range info {
		var ai AdditionalInfo

		if err := json.Unmarshal([]byte(e), &ai); err != nil {
			return nil, err
		}

		ai.Beer = true

		data, err := json.Marshal(ai)
		if err != nil {
			return nil, err
		}

		info[i] = string(data)
	}

	return info, nil
}

func (db *DB) AddAdditionalInfo(info []string, ai AdditionalInfo) ([]string, error) {
	data, err := json.Marshal(ai)
	if err != nil {
		return nil, err
	}

	info = append(info, string(data))

	return info, nil
}

func (db *DB) GetZakladkiStruct() ([]Zakladka, error) {
	var z []Zakladka

	if res := db.db.Table("zakladki").Find(&z); res.Error != nil {
		return nil, res.Error
	}

	return z, nil
}

func (db *DB) GetZakladkiZones() ([]ZakladkaZone, error) {
	var z []ZakladkaZone

	if res := db.db.Table("zakladki").Select("zakladki.id, zones.country").Joins("join zones on zones.id = zakladki.zone_id").Scan(&z); res.Error != nil {
		return nil, res.Error
	}

	return z, nil
}

func (db *DB) InsertZakladka(z Zakladka) error {
	if res := db.db.Create(&z); res.Error != nil {
		return res.Error
	}

	return nil
}

func (db *DB) UpdateZakladkaZone(zakladkaID, zoneID int) error {
	if res := db.db.Table("zakladki").Where("id = ?", zakladkaID).Update("zone_id", zoneID); res.Error != nil {
		return res.Error
	}

	return nil
}

func (db *DB) DeleteZakladka(zakladkaID int) error {
	if res := db.db.Table("zakladki").Where("id = ?", zakladkaID).Delete(&Zakladka{}); res.Error != nil {
		return res.Error
	}

	return nil
}
