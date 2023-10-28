package main

import (
	"context"
	"fmt"
	"github.com/neo4j/neo4j-go-driver/v5/neo4j"
)

type DB struct {
	driver  neo4j.DriverWithContext
	session neo4j.SessionWithContext
}

type DBConfig struct {
	URI      string
	User     string
	Password string
	DBName   string
}

func New(cfg DBConfig) (*DB, error) {
	db := new(DB)
	var err error

	db.driver, err = neo4j.NewDriverWithContext(cfg.URI, neo4j.BasicAuth(cfg.User, cfg.Password, ""))
	if err != nil {
		return nil, err
	}

	db.session = db.driver.NewSession(context.Background(), neo4j.SessionConfig{DatabaseName: cfg.DBName})

	return db, nil
}

func (db *DB) Close(ctx context.Context) {
	db.session.Close(ctx)
	db.driver.Close(ctx)
}

func (db *DB) AddParents(ctx context.Context, parents []Parent) error {
	query := `
			MERGE (parent:Person { name: $parentName, gender: $parentGender })
			MERGE (child:Person { name: $childName, gender: $childGender })
			MERGE (parent)-[:PARENT]->(child)
			RETURN parent, child`

	_, err := db.session.ExecuteWrite(ctx,
		func(tx neo4j.ManagedTransaction) (any, error) {
			for _, p := range parents {
				if _, err := db.session.Run(ctx, query, map[string]any{
					"parentName":   p.Parent.Name,
					"parentGender": p.Parent.Gender,
					"childName":    p.Child.Name,
					"childGender":  p.Child.Gender,
				}); err != nil {
					return nil, err
				}
			}

			return nil, nil
		},
	)

	return err
}

func (db *DB) FindParents(ctx context.Context, name string) ([]Person, error) {
	query := `
			MATCH (child:Person)<-[:PARENT]-(parent:Person)
			WHERE child.name = $person_name
			RETURN parent.name, parent.gender`

	result, err := db.session.Run(ctx, query, map[string]any{
		"person_name": name,
	})
	if err != nil {
		return nil, err
	}

	return toPersons(ctx, result), nil
}

func (db *DB) FindPartners(ctx context.Context, name string) ([]Person, error) {
	query := `
			MATCH (child:Person)<-[:PARENT]-(parent1:Person)
			MATCH (child:Person)<-[:PARENT]-(parent2:Person)
			WHERE parent1.name <> parent2.name AND parent1.name = $person_name
			RETURN DISTINCT parent2.name, parent2.gender`

	result, err := db.session.Run(ctx, query, map[string]any{
		"person_name": name,
	})
	if err != nil {
		return nil, err
	}

	return toPersons(ctx, result), nil
}

func (db *DB) FindSiblings(ctx context.Context, name string) ([]Person, error) {
	query := `
			MATCH (child1:Person)<-[:PARENT]-(parent1:Person)
			MATCH (child2:Person)<-[:PARENT]-(parent1:Person)
			MATCH (child1:Person)<-[:PARENT]-(parent2:Person)
			MATCH (child2:Person)<-[:PARENT]-(parent2:Person)
			WHERE parent1.gender = 'male' AND parent2.gender = 'female' AND child1.name = $person_name AND child2.name <> $person_name
			RETURN child2.name, child2.gender`

	result, err := db.session.Run(ctx, query, map[string]any{
		"person_name": name,
	})
	if err != nil {
		return nil, err
	}

	return toPersons(ctx, result), nil
}

func (db *DB) FindGrandmasN(ctx context.Context, name string, n int) ([]Person, error) {
	query := fmt.Sprintf(`
			MATCH (grand:Person)-[:PARENT*%d]->(child:Person)
			WHERE child.name = $person_name AND grand.gender = 'female'
			RETURN grand.name, grand.gender`, n+1)

	result, err := db.session.Run(ctx, query, map[string]any{
		"person_name": name,
	})
	if err != nil {
		return nil, err
	}

	return toPersons(ctx, result), nil
}

func toPersons(ctx context.Context, result neo4j.ResultWithContext) []Person {
	persons := make([]Person, 0)

	for result.Next(ctx) {
		persons = append(persons, Person{
			Name:   result.Record().Values[0].(string),
			Gender: result.Record().Values[1].(string),
		})
	}

	return persons
}
