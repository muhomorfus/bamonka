package ports

import (
	"context"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"github.com/google/uuid"
)

type TeamRepository interface {
	Create(ctx context.Context, team models.Team) (*models.Team, error)
	Update(ctx context.Context, team models.Team) error
	Delete(ctx context.Context, id uuid.UUID) error
	Get(ctx context.Context, id uuid.UUID) (*models.Team, error)
	GetAll(ctx context.Context) ([]models.Team, error)

	ListSimilarByName(ctx context.Context, substr string) ([]models.Team, error)
}

type EmployeeRepository interface {
	Create(ctx context.Context, emp models.Employee) (*models.Employee, error)
	Update(ctx context.Context, emp models.Employee) error
	SoftDelete(ctx context.Context, id uuid.UUID) error
	Delete(ctx context.Context, id uuid.UUID) error

	GetByID(ctx context.Context, id uuid.UUID) (*models.Employee, error)
	GetAll(ctx context.Context) ([]models.Employee, error)
	GetByNickname(ctx context.Context, nickname string) (*models.Employee, error)
	GetByCredentials(ctx context.Context, nickname, password string) (*models.Employee, error)
	GetByDepartment(ctx context.Context, id uuid.UUID) ([]models.Employee, error)
	GetByTeam(ctx context.Context, id uuid.UUID) ([]models.Employee, error)

	ListSimilarByName(ctx context.Context, substr string) ([]models.Employee, error)
	ListSimilarByNickname(ctx context.Context, substr string) ([]models.Employee, error)
}

type DepartmentRepository interface {
	Create(ctx context.Context, dep models.Department) (*models.Department, error)
	Update(ctx context.Context, dep models.Department) error
	Delete(ctx context.Context, id uuid.UUID) error
	Get(ctx context.Context, id uuid.UUID) (*models.Department, error)
	GetAll(ctx context.Context) ([]models.Department, error)
	Breadcrumbs(ctx context.Context, id uuid.UUID) ([]models.DepartmentShort, error)

	Children(ctx context.Context, id uuid.UUID) ([]models.Department, error)

	ListSimilarByName(ctx context.Context, substr string) ([]models.Department, error)
}

type VacationRepository interface {
	GetList(ctx context.Context, id uuid.UUID) ([]models.Vacation, error)
	Create(ctx context.Context, vac models.Vacation) (*models.Vacation, error)
}

type SubscriptionRepository interface {
	IsSubscribed(ctx context.Context, subscriber, author uuid.UUID) (bool, error)
	Subscribe(ctx context.Context, subscriber, author uuid.UUID) error
	Unsubscribe(ctx context.Context, subscriber, author uuid.UUID) error
}
