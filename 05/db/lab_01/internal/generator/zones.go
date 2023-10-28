package generator

import (
	"git.parasha.space/go/libs/faker"
	"github.com/icrowley/fake"
)

type Zone struct {
	ID             int    `csv:"id"`
	CodeName       string `csv:"code_name"`
	Country        string `csv:"country"`
	CapitalAddress string `csv:"capital_address"`
	TutorID        int    `csv:"tutor_id"`
}

func (e Zone) equal(c Zone) bool {
	return e.CapitalAddress == c.CapitalAddress || e.ID == c.ID || e.CodeName == c.CodeName
}

func (e Zone) equalMany(cs []Zone) bool {
	for _, c := range cs {
		if e.equal(c) {
			return true
		}
	}

	return false
}

func genZone(zones []Zone, staff []Employee) Zone {
	var z Zone
	first := true

	for z.equalMany(zones) || first {
		first = false

		z.ID = len(zones) + 1
		z.Country, _ = faker.Country("en")
		z.CapitalAddress, _ = faker.FullAddress("en")
		z.CodeName = fake.CharactersN(10)
		z.TutorID = faker.ArrayElement(staff).ID
	}

	return z
}

func genZones(n int, staff []Employee) ([]Zone, error) {
	managers := make([]Employee, 0, len(staff))
	for _, e := range staff {
		if e.Position == "Manager" && e.Alive {
			managers = append(managers, e)
		}
	}

	result := make([]Zone, 0, n)

	for i := 0; i < n; i++ {
		result = append(result, genZone(result, managers))
	}

	return result, nil
}
