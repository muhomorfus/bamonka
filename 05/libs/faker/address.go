package faker

import (
	"fmt"
	"github.com/icrowley/fake"
)

// FullAddress generates new full address (city + street) with given language.
func FullAddress(lang string) (string, error) {
	err := fake.SetLang(lang)
	if err != nil {
		return "", err
	}

	return fmt.Sprintf("%s, %s", fake.City(), fake.StreetAddress()), nil
}

// Country generates country with given language.
func Country(lang string) (string, error) {
	err := fake.SetLang(lang)
	if err != nil {
		return "", err
	}

	return fake.Country(), nil
}
