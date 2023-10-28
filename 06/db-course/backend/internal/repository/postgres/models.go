package postgres

import (
	"encoding/json"
	"fmt"
	"time"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"github.com/google/uuid"
	"github.com/samber/lo"
)

type Team struct {
	UUID        uuid.UUID `gorm:"primaryKey"`
	Name        string
	Description string
}

func (t Team) toModel() models.Team {
	return models.Team{
		ID:          t.UUID,
		Name:        t.Name,
		Description: t.Description,
	}
}

type Department struct {
	UUID        uuid.UUID `gorm:"primaryKey"`
	Name        string
	Description string
	ParentUUID  uuid.UUID
}

func (d Department) toShort() models.DepartmentShort {
	return models.DepartmentShort{
		ID:   d.UUID,
		Name: d.Name,
	}
}

func (d Department) toModel() models.Department {
	var parentID *uuid.UUID
	if d.ParentUUID != uuid.Nil {
		parentID = &d.ParentUUID
	}

	return models.Department{
		DepartmentShort: d.toShort(),
		Description:     d.Description,
		ParentID:        parentID,
	}
}

type Employee struct {
	UUID           uuid.UUID `gorm:"primaryKey"`
	Name           string
	Description    string
	Nickname       string
	DepartmentUUID uuid.UUID
	Position       string
	Email          string
	Phone          string
	Social         string
	BossUUID       uuid.UUID
	CreatedAt      time.Time
	DeletedAt      *time.Time
	Level          string
	Password       string
	Photo          string
}

func EmployeeFromModel(e models.Employee) (Employee, error) {
	socials, err := json.Marshal(e.Socials)
	if err != nil {
		return Employee{}, fmt.Errorf("marshal socials: %w", err)
	}

	bossID := uuid.Nil
	if e.BossID != nil {
		bossID = *e.BossID
	}

	return Employee{
		UUID:           e.ID,
		Name:           e.Name,
		Description:    e.Description,
		Nickname:       e.Nickname,
		DepartmentUUID: e.Department.ID,
		Position:       e.Position,
		Email:          e.Email,
		Phone:          e.Phone,
		Social:         string(socials),
		BossUUID:       bossID,
		CreatedAt:      e.Created,
		Level:          e.Level.String(),
		Password:       e.PasswordHash,
		Photo:          e.Photo,
	}, nil
}

type Subscription struct {
	UUID           uuid.UUID `gorm:"primaryKey"`
	AuthorUUID     uuid.UUID
	SubscriberUUID uuid.UUID
}

type Vacation struct {
	UUID         uuid.UUID `gorm:"primaryKey"`
	EmployeeUUID uuid.UUID
	Description  string
	Start, End   time.Time
}

type Work struct {
	UUID         uuid.UUID `gorm:"primaryKey"`
	EmployeeUUID uuid.UUID
	TeamUUID     uuid.UUID
	Role         string
	CreatedAt    time.Time
}

func WorkFromModel(id uuid.UUID, e models.TeamRole) Work {
	return Work{
		UUID:         uuid.New(),
		EmployeeUUID: id,
		TeamUUID:     e.Team.ID,
		Role:         e.Role,
		CreatedAt:    e.From,
	}
}

type WorkTeam struct {
	Work
	Name        string
	Description string
}

type EmployeeDepartment struct {
	Employee
	DepartmentParentUUID  uuid.UUID
	DepartmentDescription string
	DepartmentName        string
}

func (e EmployeeDepartment) ToModel(works []WorkTeam) (models.Employee, error) {
	var socials map[string]string
	if err := json.Unmarshal([]byte(e.Social), &socials); err != nil {
		return models.Employee{}, fmt.Errorf("parse socials: %w", err)
	}

	var bossID *uuid.UUID
	if e.BossUUID != uuid.Nil {
		bossID = &e.BossUUID
	}

	var parentID *uuid.UUID
	if e.DepartmentParentUUID != uuid.Nil {
		parentID = &e.DepartmentParentUUID
	}

	return models.Employee{
		ID:       e.UUID,
		Name:     e.Name,
		Nickname: e.Nickname,
		Department: models.Department{
			DepartmentShort: models.DepartmentShort{
				ID:   e.DepartmentUUID,
				Name: e.DepartmentName,
			},
			Description: e.DepartmentDescription,
			ParentID:    parentID,
		},
		Position:    e.Position,
		Email:       e.Email,
		Phone:       e.Phone,
		Socials:     socials,
		BossID:      bossID,
		Description: e.Description,
		Teams: lo.Map(works, func(w WorkTeam, _ int) models.TeamRole {
			return models.TeamRole{
				Team: models.Team{
					ID:          w.TeamUUID,
					Name:        w.Name,
					Description: w.Description,
				},
				Role: w.Role,
				From: w.CreatedAt,
			}
		}),
		Created:      e.CreatedAt,
		Working:      e.DeletedAt == nil,
		Level:        models.LevelFromString(e.Level),
		PasswordHash: e.Password,
		Photo:        e.Photo,
	}, nil
}
