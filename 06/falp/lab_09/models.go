package main

type Person struct {
	Name   string
	Gender string
}

func Male(name string) Person {
	return Person{
		Name:   name,
		Gender: "male",
	}
}

func Female(name string) Person {
	return Person{
		Name:   name,
		Gender: "female",
	}
}

type Parent struct {
	Parent Person
	Child  Person
}
