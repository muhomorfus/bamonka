package managers

import (
	"context"
	"fmt"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/contextutils"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/ports"
	"github.com/google/uuid"
)

type Config struct {
	dep ports.DepartmentManager
	emp ports.ProfileManager

	name, user, password string
	department           string
}

func NewConfig(dep ports.DepartmentManager, emp ports.ProfileManager, name string, user string, password string, department string) *Config {
	return &Config{dep: dep, emp: emp, name: name, user: user, password: password, department: department}
}

func (c *Config) Init(ctx context.Context) error {
	ctx = contextutils.SetEmployee(ctx, &models.Employee{Level: models.Admin})

	deps, err := c.dep.GetAll(ctx)
	if err != nil {
		return fmt.Errorf("get all departments: %w", err)
	}

	emps, err := c.emp.GetAll(ctx)
	if err != nil {
		return fmt.Errorf("get all employee: %w", err)
	}

	if len(deps) != 0 && len(emps) != 0 {
		return nil
	}

	dep, err := c.dep.Create(ctx, models.Department{
		DepartmentShort: models.DepartmentShort{
			ID:   uuid.New(),
			Name: c.department,
		},
	})
	if err != nil {
		return fmt.Errorf("create initial department: %w", err)
	}

	_, err = c.emp.Create(ctx, models.Employee{
		ID:         uuid.New(),
		Name:       c.name,
		Nickname:   c.user,
		Department: *dep,
		Socials:    make(map[string]string),
		Level:      models.Admin,
	}, c.password)
	if err != nil {
		return fmt.Errorf("create initial department: %w", err)
	}

	return nil
}
