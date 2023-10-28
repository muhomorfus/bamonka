package main

import (
	"context"
	"fmt"
	"log"
)

func main() {
	db, err := New(DBConfig{
		URI:      "neo4j+s://3e239a2c.databases.neo4j.io",
		User:     "neo4j",
		Password: "yJ-brjyZ0ERmNvBxSTWxJLg-7Up8Lux_AtEFvXscMR8",
		DBName:   "neo4j",
	})
	if err != nil {
		log.Fatalln(err)
	}
	defer db.Close(context.TODO())

	ctx := context.Background()

	if err := fillDB(ctx, db); err != nil {
		log.Fatalln(err)
	}

	persons, err := db.FindParents(ctx, "eliza")
	if err != nil {
		log.Fatalln(err)
	}
	fmt.Printf("%+v\n", persons)
	fmt.Println("----------------------------------------------------------------------")

	persons, err = db.FindSiblings(ctx, "eliza")
	if err != nil {
		log.Fatalln(err)
	}
	fmt.Printf("%+v\n", persons)
	fmt.Println("----------------------------------------------------------------------")

	persons, err = db.FindPartners(ctx, "marianna")
	if err != nil {
		log.Fatalln(err)
	}
	fmt.Printf("%+v\n", persons)
	fmt.Println("----------------------------------------------------------------------")

	persons, err = db.FindGrandmasN(ctx, "eliza", 2)
	if err != nil {
		log.Fatalln(err)
	}
	fmt.Printf("%+v\n", persons)
	fmt.Println("----------------------------------------------------------------------")
}

func fillDB(ctx context.Context, db *DB) error {
	eliza := Female("eliza")
	dabby := Female("dabby")
	donnie := Male("donnie")

	marianna := Female("marianna")
	nigel := Male("nigel")

	sofie := Female("sofie")

	sir := Male("sir")
	sirness := Female("sirness")

	grandsir := Male("grandsir")
	grandsirness := Female("grandsirness")

	grandgrandsirness := Female("grandgrandsirness")

	relations := []Parent{
		{marianna, eliza},
		{nigel, eliza},
		{marianna, dabby},
		{nigel, dabby},
		{marianna, donnie},
		{nigel, donnie},

		{sofie, marianna},

		{sir, nigel},
		{sirness, nigel},

		{grandsir, sirness},
		{grandsirness, sirness},

		{grandgrandsirness, grandsirness},
	}

	return db.AddParents(ctx, relations)
}
