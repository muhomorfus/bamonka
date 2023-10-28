package generator

type SafetyClass struct {
	ID             int    `csv:"id"`
	Name           string `csv:"name"`
	NumOfGuardians int    `csv:"num_of_guardians"`
	AirDefense     bool   `csv:"have_air_defense"`
}

func genSafetyClasses() ([]SafetyClass, error) {
	return []SafetyClass{
		{
			ID:             1,
			Name:           "магазинные склад",
			NumOfGuardians: 2,
			AirDefense:     false,
		},
		{
			ID:             2,
			Name:           "небольшое хранилище",
			NumOfGuardians: 3,
			AirDefense:     false,
		},
		{
			ID:             3,
			Name:           "укрытие",
			NumOfGuardians: 5,
			AirDefense:     false,
		},
		{
			ID:             4,
			Name:           "опорная точка",
			NumOfGuardians: 7,
			AirDefense:     false,
		},
		{
			ID:             5,
			Name:           "крупное хранилище",
			NumOfGuardians: 10,
			AirDefense:     false,
		},
		{
			ID:             6,
			Name:           "бомбоубежище",
			NumOfGuardians: 20,
			AirDefense:     false,
		},
		{
			ID:             7,
			Name:           "региональное хранилище",
			NumOfGuardians: 30,
			AirDefense:     false,
		},
		{
			ID:             8,
			Name:           "особо охраняемый объект",
			NumOfGuardians: 100,
			AirDefense:     true,
		},
	}, nil
}
