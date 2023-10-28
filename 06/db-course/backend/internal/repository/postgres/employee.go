package postgres

import (
	"context"
	"fmt"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/roledb"
	"time"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"github.com/google/uuid"
	"github.com/samber/lo"
	"gorm.io/gorm"
)

type EmployeeRepository struct {
	*roledb.RoleDB
}

func NewEmployeeRepository(roleDB *roledb.RoleDB) *EmployeeRepository {
	return &EmployeeRepository{
		RoleDB: roleDB,
	}
}

func (e *EmployeeRepository) Create(ctx context.Context, emp models.Employee) (*models.Employee, error) {
	repoEmp, err := EmployeeFromModel(emp)
	if err != nil {
		return nil, fmt.Errorf("convert employee: %w", err)
	}

	repoWorks := lo.Map(emp.Teams, func(team models.TeamRole, _ int) Work {
		return WorkFromModel(emp.ID, team)
	})

	if err := e.DB(ctx).Table("employee").Create(&repoEmp).Error; err != nil {
		return nil, fmt.Errorf("create employee in db: %w", err)
	}

	for _, work := range repoWorks {
		if err := e.DB(ctx).Table("work").Create(&work).Error; err != nil {
			return nil, fmt.Errorf("create employee work in db: %w", err)
		}
	}

	return &emp, nil
}

func changes(old, new []Work) ([]Work, []Work, []Work) {
	mapOld := lo.SliceToMap(old, func(w Work) (string, Work) {
		return w.EmployeeUUID.String() + w.TeamUUID.String(), w
	})

	mapNew := lo.SliceToMap(new, func(w Work) (string, Work) {
		return w.EmployeeUUID.String() + w.TeamUUID.String(), w
	})

	creations := make([]Work, 0, len(new))
	updations := make([]Work, 0, len(new))
	deletions := make([]Work, 0, len(old))

	for k, v := range mapOld {
		m, ok := mapNew[k]
		switch {
		case ok && v.UUID == m.UUID:
			updations = append(updations, m)
		case !ok:
			deletions = append(updations, v)
		}
	}

	for k, v := range mapNew {
		if _, ok := mapOld[k]; !ok {
			creations = append(creations, v)
		}
	}

	return creations, updations, deletions
}

func (e *EmployeeRepository) Update(ctx context.Context, emp models.Employee) error {
	repoEmp, err := EmployeeFromModel(emp)
	if err != nil {
		return fmt.Errorf("convert employee: %w", err)
	}

	if err := e.DB(ctx).Table("employee").
		Where("uuid = ?", repoEmp.UUID).
		Save(&repoEmp).Error; err != nil {
		return fmt.Errorf("save employee: %w", err)
	}

	repoWorks := lo.Map(emp.Teams, func(team models.TeamRole, _ int) Work {
		return WorkFromModel(emp.ID, team)
	})

	var curWorks []Work
	if err := e.DB(ctx).Table("work").
		Where("employee_uuid = ?", repoEmp.UUID).
		Find(&curWorks).Error; err != nil {
		return fmt.Errorf("get current works: %w", err)
	}

	crt, upd, del := changes(curWorks, repoWorks)

	for _, work := range crt {
		work.CreatedAt = time.Now().UTC()

		if err := e.DB(ctx).Table("work").
			Create(&work).Error; err != nil {
			return fmt.Errorf("create employee work in db: %w", err)
		}
	}

	for _, work := range upd {
		if err := e.DB(ctx).Table("work").
			Where("uuid = ?", work.UUID).Save(&work).Error; err != nil {
			return fmt.Errorf("save employee work in db: %w", err)
		}
	}

	for _, work := range del {
		if err := e.DB(ctx).Table("work").
			Where("uuid = ?", work.UUID).Delete(&work).Error; err != nil {
			return fmt.Errorf("delete employee work in db: %w", err)
		}
	}

	return nil
}

func (e *EmployeeRepository) SoftDelete(ctx context.Context, id uuid.UUID) error {
	if err := e.DB(ctx).Table("employee").Where("uuid = ?", id).
		UpdateColumn("deleted_at", time.Now().UTC()).Error; err != nil {
		return fmt.Errorf("soft delete in db: %w", err)
	}

	return nil
}

func (e *EmployeeRepository) Delete(ctx context.Context, id uuid.UUID) error {
	if err := e.DB(ctx).Table("employee").Where("uuid = ?", id).
		Delete(&Employee{}).Error; err != nil {
		return fmt.Errorf("soft delete in db: %w", err)
	}

	return nil
}

func (e *EmployeeRepository) employeeQuery(ctx context.Context) *gorm.DB {
	db := e.DB(ctx).Table("employee")
	db = db.Select("employee.*, department.parent_uuid as department_parent_uuid, department.name as department_name, department.description as department_description")
	db = db.Joins("join department on employee.department_uuid = department.uuid")

	return db
}

func (e *EmployeeRepository) workQuery(ctx context.Context, id uuid.UUID) *gorm.DB {
	db := e.DB(ctx).Table("work")
	db = db.Select("work.*, team.name as name, team.description as description")
	db = db.Joins("join team on work.team_uuid = team.uuid")
	db = db.Where("work.employee_uuid = ?", id)

	return db
}

func (e *EmployeeRepository) convertEmployee(ctx context.Context, repoEmp EmployeeDepartment) (models.Employee, error) {
	var repoWorks []WorkTeam
	if err := e.workQuery(ctx, repoEmp.UUID).Find(&repoWorks).Error; err != nil {
		return models.Employee{}, fmt.Errorf("get works from db: %w", err)
	}

	emp, err := repoEmp.ToModel(repoWorks)
	if err != nil {
		return models.Employee{}, fmt.Errorf("convert employee from db: %w", err)
	}

	return emp, nil
}

func (e *EmployeeRepository) GetByID(ctx context.Context, id uuid.UUID) (*models.Employee, error) {
	var repoEmp EmployeeDepartment

	if err := e.employeeQuery(ctx).Where("employee.uuid = ?", id).Take(&repoEmp).Error; err != nil {
		return nil, fmt.Errorf("get employee from db: %w", err)
	}

	emp, err := e.convertEmployee(ctx, repoEmp)
	if err != nil {
		return nil, fmt.Errorf("convert employee from db: %w", err)
	}

	return &emp, nil
}

func (e *EmployeeRepository) GetByNickname(ctx context.Context, nickname string) (*models.Employee, error) {
	var repoEmp EmployeeDepartment

	if err := e.employeeQuery(ctx).Where("employee.nickname = ?", nickname).
		Take(&repoEmp).Error; err != nil {
		return nil, fmt.Errorf("get employee from db: %w", err)
	}

	emp, err := e.convertEmployee(ctx, repoEmp)
	if err != nil {
		return nil, fmt.Errorf("convert employee from db: %w", err)
	}

	return &emp, nil
}

func (e *EmployeeRepository) GetByCredentials(ctx context.Context, nickname, password string) (*models.Employee, error) {
	var repoEmp EmployeeDepartment

	if err := e.employeeQuery(ctx).Where("employee.nickname = ?", nickname).
		Where("password = ?", password).
		Where("deleted_at is null").
		Take(&repoEmp).Error; err != nil {
		return nil, fmt.Errorf("get employee from db: %w", err)
	}

	emp, err := e.convertEmployee(ctx, repoEmp)
	if err != nil {
		return nil, fmt.Errorf("convert employee from db: %w", err)
	}

	return &emp, nil
}

func (e *EmployeeRepository) ListSimilarByName(ctx context.Context, substr string) ([]models.Employee, error) {
	var repoEmp []EmployeeDepartment

	if err := e.employeeQuery(ctx).
		Where("lower(employee.name) like ?", "%"+substr+"%").
		Find(&repoEmp).Error; err != nil {
		return nil, fmt.Errorf("search employee from db by name: %w", err)
	}

	emps := make([]models.Employee, len(repoEmp))
	var err error
	for i, re := range repoEmp {
		emps[i], err = e.convertEmployee(ctx, re)
		if err != nil {
			return nil, fmt.Errorf("convert employee from db: %w", err)
		}
	}

	return emps, nil
}

func (e *EmployeeRepository) ListSimilarByNickname(ctx context.Context, substr string) ([]models.Employee, error) {
	var repoEmp []EmployeeDepartment

	if err := e.employeeQuery(ctx).
		Where("lower(employee.nickname) like ?", "%"+substr+"%").
		Find(&repoEmp).Error; err != nil {
		return nil, fmt.Errorf("search employee from db by nick: %w", err)
	}

	emps := make([]models.Employee, len(repoEmp))
	var err error
	for i, re := range repoEmp {
		emps[i], err = e.convertEmployee(ctx, re)
		if err != nil {
			return nil, fmt.Errorf("convert employee from db: %w", err)
		}
	}

	return emps, nil
}

func (e *EmployeeRepository) GetAll(ctx context.Context) ([]models.Employee, error) {
	var repoEmp []EmployeeDepartment

	if err := e.employeeQuery(ctx).Find(&repoEmp).Error; err != nil {
		return nil, fmt.Errorf("get all employee from db: %w", err)
	}

	emps := make([]models.Employee, len(repoEmp))
	var err error
	for i, re := range repoEmp {
		emps[i], err = e.convertEmployee(ctx, re)
		if err != nil {
			return nil, fmt.Errorf("convert employee from db: %w", err)
		}
	}

	return emps, nil
}

func (e *EmployeeRepository) GetByDepartment(ctx context.Context, id uuid.UUID) ([]models.Employee, error) {
	var repoEmp []EmployeeDepartment

	if err := e.employeeQuery(ctx).Where("employee.department_uuid = ?", id).Find(&repoEmp).Error; err != nil {
		return nil, fmt.Errorf("get employee from db: %w", err)
	}

	emps := make([]models.Employee, len(repoEmp))
	var err error
	for i, re := range repoEmp {
		emps[i], err = e.convertEmployee(ctx, re)
		if err != nil {
			return nil, fmt.Errorf("convert employee from db: %w", err)
		}
	}

	return emps, nil
}

func (e *EmployeeRepository) GetByTeam(ctx context.Context, id uuid.UUID) ([]models.Employee, error) {
	var repoEmp []EmployeeDepartment

	if err := e.employeeQuery(ctx).
		InnerJoins("inner join work on employee.uuid = work.employee_uuid").
		Where("work.team_uuid = ?", id).
		Find(&repoEmp).Error; err != nil {
		return nil, fmt.Errorf("get employee from db: %w", err)
	}

	emps := make([]models.Employee, len(repoEmp))
	var err error
	for i, re := range repoEmp {
		emps[i], err = e.convertEmployee(ctx, re)
		if err != nil {
			return nil, fmt.Errorf("convert employee from db: %w", err)
		}
	}

	return emps, nil
}
