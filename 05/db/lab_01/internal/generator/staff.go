package generator

import (
	"git.parasha.space/go/libs/faker"
	"time"
)

var positions = []string{
	"Soldier",
	"Soldier",
	"Soldier",
	"Soldier",
	"Soldier",
	"Medical",
	"Manager",
	"Manager",
	"Manager",
	"Support",
}

var alives = []bool{
	true,
	true,
	true,
	true,
	true,
	false,
}

type Employee struct {
	ID        int          `csv:"id"`
	Nickname  string       `csv:"nickname"`
	BirthDate postgresDate `csv:"birth_date"`
	Position  string       `csv:"position"`
	Alive     bool         `csv:"is_alive"`
	BossID    *int         `csv:"boss_id"`
}

func (e Employee) equal(c Employee) bool {
	return e.Nickname == c.Nickname || e.ID == c.ID
}

func (e Employee) equalMany(cs []Employee) bool {
	for _, c := range cs {
		if e.equal(c) {
			return true
		}
	}

	return false
}

func genEmployee(empls []Employee) Employee {
	var e Employee
	first := true

	for e.equalMany(empls) || first {
		first = false

		e.ID = len(empls) + 1
		e.Alive = faker.ArrayElement(alives)
		e.BirthDate = postgresDate(faker.Time(faker.NewTimeLimitYear(1980, 2002)))
		e.Nickname = faker.Login()
		e.BossID = intPtr(faker.ArrayElement(empls).ID)
		e.Position = faker.ArrayElement(positions)
	}

	return e
}

func genStaff(n int) ([]Employee, error) {
	result := make([]Employee, 0, n)
	result = append(result, Employee{
		ID:        1,
		Nickname:  "muhomorfus",
		BirthDate: postgresDate(time.Date(2002, 06, 23, 0, 0, 0, 0, time.UTC)),
		Position:  "Mega boss",
		Alive:     true,
		BossID:    nil,
	})

	for i := 1; i < n; i++ {
		result = append(result, genEmployee(result))
	}

	return result, nil
}
