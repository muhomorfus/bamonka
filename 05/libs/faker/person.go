package faker

import (
	"fmt"
	"github.com/icrowley/fake"
)

// Gender represents person's gender.
// Yes, there are two genders.
type Gender string

const (
	Male   Gender = "male"
	Female Gender = "female"
)

// PersonInfo represents person info.
type PersonInfo struct {
	Name       string
	Surname    string
	Patronymic string
	Gender     Gender
}

func gender() Gender {
	if Bool() {
		return Male
	}

	return Female
}

// Person generates personal base info.
func Person(lang string) (*PersonInfo, error) {
	if err := fake.SetLang(lang); err != nil {
		return nil, err
	}

	info := new(PersonInfo)
	info.Gender = gender()

	if info.Gender == Male {
		info.Name = fake.MaleFirstName()
		info.Surname = fake.MaleLastName()
		info.Patronymic = fake.MalePatronymic()
	} else {
		info.Name = fake.FemaleFirstName()
		info.Surname = fake.FemaleLastName()
		info.Patronymic = fake.FemalePatronymic()
	}

	return info, nil
}

// RussianMobilePhone generates +7 ___ ___-__-__ phone number.
func RussianMobilePhone() string {
	return fmt.Sprintf(
		"+7 %03d %03d-%02d-%02d",
		IntWithLimits(900, 1000),
		IntWithLimits(0, 1000),
		IntWithLimits(0, 100),
		IntWithLimits(0, 100),
	)
}

// Login generates random login.
func Login() string {
	return fake.UserName()
}
