package postgres

import (
	"context"
	"fmt"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/roledb"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"github.com/AlekSi/pointer"
	"github.com/google/uuid"
	"github.com/samber/lo"
)

type DepartmentRepository struct {
	*roledb.RoleDB
}

func NewDepartmentRepository(roleDB *roledb.RoleDB) *DepartmentRepository {
	return &DepartmentRepository{
		RoleDB: roleDB,
	}
}

func (d *DepartmentRepository) Create(ctx context.Context, dep models.Department) (*models.Department, error) {
	parentID := uuid.Nil
	if dep.ParentID != nil {
		parentID = *dep.ParentID
	}

	repoDep := Department{
		UUID:        dep.ID,
		Name:        dep.Name,
		Description: dep.Description,
		ParentUUID:  parentID,
	}

	if err := d.DB(ctx).Table("department").Create(&repoDep).Error; err != nil {
		return nil, fmt.Errorf("create department in db: %w", err)
	}

	return &dep, nil
}

func (d *DepartmentRepository) Update(ctx context.Context, dep models.Department) error {
	parentID := uuid.Nil
	if dep.ParentID != nil {
		parentID = *dep.ParentID
	}

	repoDep := Department{
		UUID:        dep.ID,
		Name:        dep.Name,
		Description: dep.Description,
		ParentUUID:  parentID,
	}

	if err := d.DB(ctx).Table("department").
		Where("uuid = ?", repoDep.UUID).
		Save(&repoDep).Error; err != nil {
		return fmt.Errorf("save department in db: %w", err)
	}

	return nil
}

func (d *DepartmentRepository) Delete(ctx context.Context, id uuid.UUID) error {
	if err := d.DB(ctx).Table("department").
		Where("uuid = ?", id).
		Delete(&Department{}).Error; err != nil {
		return fmt.Errorf("delete department on db: %w", err)
	}

	return nil
}

func (d *DepartmentRepository) Get(ctx context.Context, id uuid.UUID) (*models.Department, error) {
	var repoDep Department
	if err := d.DB(ctx).Table("department").
		Where("uuid = ?", id).
		Take(&repoDep).Error; err != nil {
		return nil, fmt.Errorf("get department from db: %w", err)
	}

	return pointer.To(repoDep.toModel()), nil
}

func (d *DepartmentRepository) Breadcrumbs(ctx context.Context, id uuid.UUID) ([]models.DepartmentShort, error) {
	var repoDeps []Department
	if err := d.DB(ctx).Raw("select * from department_breadcrumbs(?)", id).
		Scan(&repoDeps).Error; err != nil {
		return nil, fmt.Errorf("get departmemt breadcrumbs: %w", err)
	}

	return lo.Map(repoDeps, func(e Department, _ int) models.DepartmentShort {
		return e.toShort()
	}), nil
}

func (d *DepartmentRepository) Children(ctx context.Context, id uuid.UUID) ([]models.Department, error) {
	var repoDeps []Department
	if err := d.DB(ctx).Table("department").
		Where("parent_uuid = ?", id).
		Find(&repoDeps).Error; err != nil {
		return nil, fmt.Errorf("get departmemt children: %w", err)
	}

	return lo.Map(repoDeps, func(e Department, _ int) models.Department {
		return e.toModel()
	}), nil
}

func (d *DepartmentRepository) ListSimilarByName(ctx context.Context, substr string) ([]models.Department, error) {
	var repoDeps []Department
	if err := d.DB(ctx).Table("department").
		Where("lower(name) like ?", "%"+substr+"%").
		Find(&repoDeps).Error; err != nil {
		return nil, fmt.Errorf("get departmemt similar: %w", err)
	}

	return lo.Map(repoDeps, func(e Department, _ int) models.Department {
		return e.toModel()
	}), nil
}

func (d *DepartmentRepository) GetAll(ctx context.Context) ([]models.Department, error) {
	var repoDeps []Department
	if err := d.DB(ctx).Table("department").Find(&repoDeps).Error; err != nil {
		return nil, fmt.Errorf("get departmemt similar: %w", err)
	}

	return lo.Map(repoDeps, func(e Department, _ int) models.Department {
		return e.toModel()
	}), nil
}
