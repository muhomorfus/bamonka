package generator

import (
	"fmt"
	"git.parasha.space/go/libs/faker"
	"github.com/icrowley/fake"
)

type Consumer struct {
	ID       int    `csv:"id"`
	Nickname string `csv:"nickname"`
	Alive    bool   `csv:"is_alive"`
	Contact  string `csv:"contact"`
	ZoneID   int    `csv:"zone_id"`
}

func (e Consumer) equal(c Consumer) bool {
	return e.Nickname == c.Nickname || e.ID == c.ID || e.Contact == c.Contact
}

func (e Consumer) equalMany(cs []Consumer) bool {
	for _, c := range cs {
		if e.equal(c) {
			return true
		}
	}

	return false
}

func genConsumer(consumers []Consumer, zones []Zone) Consumer {
	var c Consumer
	first := true

	for c.equalMany(consumers) || first {
		first = false

		c.ID = len(consumers) + 1
		c.Alive = faker.ArrayElement(alives)
		c.ZoneID = faker.ArrayElement(zones).ID
		c.Nickname = faker.Login()
		c.Contact = fmt.Sprintf("msr:@%s", fake.CharactersN(faker.IntWithLimits(4, 8)))
	}

	return c
}

func genConsumers(n int, zones []Zone) ([]Consumer, error) {
	result := make([]Consumer, 0, n)

	for i := 0; i < n; i++ {
		result = append(result, genConsumer(result, zones))
	}

	return result, nil
}
