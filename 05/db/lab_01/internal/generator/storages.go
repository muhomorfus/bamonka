package generator

import (
	"git.parasha.space/go/libs/faker"
)

type Storage struct {
	ID            int    `csv:"id"`
	ZoneID        int    `csv:"zone_id"`
	SafetyClassID int    `csv:"safety_class_id"`
	MentorID      int    `csv:"mentor_id"`
	Address       string `csv:"address"`
}

func (e Storage) equal(c Storage) bool {
	return e.Address == c.Address || e.ID == c.ID
}

func (e Storage) equalMany(cs []Storage) bool {
	for _, c := range cs {
		if e.equal(c) {
			return true
		}
	}

	return false
}

func genStorage(storages []Storage, zones []Zone, staff []Employee, classes []SafetyClass) Storage {
	var s Storage
	first := true

	for s.equalMany(storages) || first {
		first = false

		s.ID = len(storages) + 1
		s.Address, _ = faker.FullAddress("en")
		s.ZoneID = faker.ArrayElement(zones).ID
		s.MentorID = faker.ArrayElement(staff).ID
		s.SafetyClassID = faker.ArrayElement(classes).ID
	}

	return s
}

func genStorages(n int, zones []Zone, staff []Employee, classes []SafetyClass) ([]Storage, error) {
	result := make([]Storage, 0, n)

	managers := make([]Employee, 0, len(staff))
	for _, e := range staff {
		if e.Position == "Manager" && e.Alive {
			managers = append(managers, e)
		}
	}

	probClasses := make([]SafetyClass, 0, 5*len(classes))
	probClasses = append(probClasses, classes[:len(classes)-1]...)
	probClasses = append(probClasses, classes[:len(classes)-1]...)
	probClasses = append(probClasses, classes[:len(classes)-1]...)
	probClasses = append(probClasses, classes[len(classes)-1])

	for i := 0; i < n; i++ {
		result = append(result, genStorage(result, zones, managers, probClasses))
	}

	return result, nil
}
