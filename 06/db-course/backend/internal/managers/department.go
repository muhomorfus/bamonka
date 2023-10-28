package managers

import (
	"context"
	"fmt"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/contextutils"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/ports"
	"github.com/google/uuid"
)

type Department struct {
	repo ports.DepartmentRepository
}

func NewDepartment(repo ports.DepartmentRepository) *Department {
	return &Department{
		repo: repo,
	}
}

func (m *Department) Create(ctx context.Context, dep models.Department) (*models.Department, error) {
	if !contextutils.HasLevel(ctx, models.Admin) {
		return nil, fmt.Errorf("check permissions: %w", models.ErrForbidden)
	}

	d, err := m.repo.Create(ctx, dep)
	if err != nil {
		return nil, fmt.Errorf("create department: %w", err)
	}

	return d, nil
}

func (m *Department) Update(ctx context.Context, dep models.Department) error {
	if !contextutils.HasLevel(ctx, models.Admin) {
		return fmt.Errorf("check permissions: %w", models.ErrForbidden)
	}

	if err := m.repo.Update(ctx, dep); err != nil {
		return fmt.Errorf("update department: %w", err)
	}

	return nil
}

func (m *Department) Delete(ctx context.Context, id uuid.UUID) error {
	if !contextutils.HasLevel(ctx, models.Admin) {
		return fmt.Errorf("check permissions: %w", models.ErrForbidden)
	}

	children, err := m.repo.Children(ctx, id)
	if err != nil {
		return fmt.Errorf("get department children: %w", err)
	}

	if len(children) != 0 {
		return fmt.Errorf("check children of department: %w", models.ErrDepartmentWithChildren)
	}

	if err := m.repo.Delete(ctx, id); err != nil {
		return fmt.Errorf("delete department: %w", err)
	}

	return nil
}

func (m *Department) Get(ctx context.Context, id uuid.UUID) (*models.Department, error) {
	if !contextutils.IsAuthorized(ctx) {
		return nil, fmt.Errorf("check permissions: %w", models.ErrNotAuthorized)
	}

	d, err := m.repo.Get(ctx, id)
	if err != nil {
		return nil, fmt.Errorf("get department: %w", err)
	}

	return d, nil
}

func (m *Department) GetAll(ctx context.Context) ([]models.Department, error) {
	if !contextutils.IsAuthorized(ctx) {
		return nil, fmt.Errorf("check permissions: %w", models.ErrNotAuthorized)
	}

	d, err := m.repo.GetAll(ctx)
	if err != nil {
		return nil, fmt.Errorf("get department: %w", err)
	}

	return d, nil
}

func (m *Department) GetDepartments(ctx context.Context, id uuid.UUID) ([]models.DepartmentShort, error) {
	if !contextutils.IsAuthorized(ctx) {
		return nil, fmt.Errorf("check permissions: %w", models.ErrNotAuthorized)
	}

	deps, err := m.repo.Breadcrumbs(ctx, id)
	if err != nil {
		return nil, fmt.Errorf("get department breadcrumbs: %w", err)
	}

	return deps, nil
}
