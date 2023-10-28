package models

import (
	"time"

	"github.com/google/uuid"
)

type Level string

const (
	User  Level = "user"
	HR    Level = "hr"
	Admin Level = "admin"
)

func (l Level) String() string {
	return string(l)
}

func LevelFromString(s string) Level {
	return Level(s)
}

type Team struct {
	ID          uuid.UUID
	Name        string
	Description string
}

type TeamRole struct {
	Team Team
	Role string
	From time.Time
}

type Department struct {
	DepartmentShort
	Description string
	ParentID    *uuid.UUID
}

type DepartmentShort struct {
	ID   uuid.UUID
	Name string
}

type Employee struct {
	ID           uuid.UUID
	Name         string
	Nickname     string
	Department   Department
	Position     string
	Email        string
	Phone        string
	Socials      map[string]string
	BossID       *uuid.UUID
	Description  string
	Teams        []TeamRole
	Created      time.Time
	Working      bool
	Level        Level
	PasswordHash string
	Photo        string
}

type Vacation struct {
	ID          uuid.UUID
	Start, End  time.Time
	Description string
	Employee    Employee
}

type SearchResult struct {
	EmployeeList   []Employee
	TeamList       []Team
	DepartmentList []Department
}
