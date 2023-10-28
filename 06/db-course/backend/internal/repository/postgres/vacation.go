package postgres

import (
	"context"
	"fmt"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/roledb"
	"time"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/ports"
	"github.com/google/uuid"
	"github.com/samber/lo"
)

type VacationRepository struct {
	*roledb.RoleDB
	employeeRepo ports.EmployeeRepository
}

func NewVacationRepository(roleDB *roledb.RoleDB, employeeRepo ports.EmployeeRepository) *VacationRepository {
	return &VacationRepository{RoleDB: roleDB, employeeRepo: employeeRepo}
}

func (v *VacationRepository) GetList(ctx context.Context, id uuid.UUID) ([]models.Vacation, error) {
	var repoVac []Vacation
	if err := v.DB(ctx).Table("vacation").Where("employee_uuid = ?", id).Where("\"end\" >= ?", time.Now().UTC()).Order("start").Find(&repoVac).Error; err != nil {
		return nil, fmt.Errorf("get vacations: %w", err)
	}

	emp, err := v.employeeRepo.GetByID(ctx, id)
	if err != nil {
		return nil, fmt.Errorf("get employee: %w", err)
	}

	return lo.Map(repoVac, func(vac Vacation, _ int) models.Vacation {
		return models.Vacation{
			ID:          vac.UUID,
			Start:       vac.Start,
			End:         vac.End,
			Description: vac.Description,
			Employee:    *emp,
		}
	}), nil
}

func (v *VacationRepository) Create(ctx context.Context, vac models.Vacation) (*models.Vacation, error) {
	repoVac := Vacation{
		UUID:         vac.ID,
		EmployeeUUID: vac.Employee.ID,
		Description:  vac.Description,
		Start:        vac.Start,
		End:          vac.End,
	}

	if err := v.DB(ctx).Table("vacation").Create(&repoVac).Error; err != nil {
		return nil, fmt.Errorf("create vacation in db: %w", err)
	}

	return &vac, nil
}
