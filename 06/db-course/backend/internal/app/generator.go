package app

import (
	"context"
	"fmt"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/contextutils"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"github.com/go-faker/faker/v4"
	"github.com/google/uuid"
	"math/rand"
	"time"
)

func (a *App) Generate(ctx context.Context) error {
	rand.Seed(time.Now().UnixNano())

	a.logger.Infow("starting generate application")

	ctx = contextutils.SetEmployee(ctx, &models.Employee{Level: models.Admin})

	var n int
	fmt.Print("n > ")
	if _, err := fmt.Scanln(&n); err != nil {
		return fmt.Errorf("scan n: %w", err)
	}

	deps, err := a.departmentRepo.GetAll(ctx)
	if err != nil {
		return fmt.Errorf("get all: %w", err)
	}
	dep := deps[0]

	for i := 0; i < n; i++ {
		_, err := a.departmentRepo.Create(ctx, models.Department{
			DepartmentShort: models.DepartmentShort{
				ID:   uuid.New(),
				Name: faker.LastName() + faker.LastName() + "Department",
			},
		})
		if err != nil {
			return fmt.Errorf("create department: %w", err)
		}

		_, err = a.teamRepo.Create(ctx, models.Team{
			ID:   uuid.New(),
			Name: faker.LastName() + faker.LastName() + "Team",
		})
		if err != nil {
			return fmt.Errorf("create team: %w", err)
		}

		_, err = a.employeeRepo.Create(ctx, models.Employee{
			ID:         uuid.New(),
			Name:       faker.FirstName() + " " + faker.LastName() + " " + faker.LastName(),
			Nickname:   faker.Username(),
			Department: dep,
			Level:      models.User,
		})
		if err != nil {
			return fmt.Errorf("create team: %w", err)
		}
	}

	return nil
}
