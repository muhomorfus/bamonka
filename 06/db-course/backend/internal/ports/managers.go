package ports

import (
	"context"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"github.com/google/uuid"
)

type DepartmentManager interface {
	Create(ctx context.Context, dep models.Department) (*models.Department, error)
	Update(ctx context.Context, dep models.Department) error
	Delete(ctx context.Context, id uuid.UUID) error
	Get(ctx context.Context, id uuid.UUID) (*models.Department, error)
	GetAll(ctx context.Context) ([]models.Department, error)
	GetDepartments(ctx context.Context, id uuid.UUID) ([]models.DepartmentShort, error)
}

type ProfileManager interface {
	Create(ctx context.Context, emp models.Employee, password string) (*models.Employee, error)
	Update(ctx context.Context, emp models.Employee) error

	Fire(ctx context.Context, id uuid.UUID) error
	Delete(ctx context.Context, id uuid.UUID) error

	GetByID(ctx context.Context, id uuid.UUID) (*models.Employee, error)
	GetByNickname(ctx context.Context, nick string) (*models.Employee, error)
	GetByJWT(ctx context.Context, token string) (*models.Employee, error)
	GetAll(ctx context.Context) ([]models.Employee, error)
	GetByDepartment(ctx context.Context, id uuid.UUID) ([]models.Employee, error)
	GetByTeam(ctx context.Context, id uuid.UUID) ([]models.Employee, error)

	Login(ctx context.Context, nickname, password string) (string, error)

	ChangeSubscription(ctx context.Context, author uuid.UUID) (bool, error)
	Subscribed(ctx context.Context, author uuid.UUID) (bool, error)

	CreateVacation(ctx context.Context, vac models.Vacation) (*models.Vacation, error)
	GetNearestVacations(ctx context.Context, id uuid.UUID) ([]models.Vacation, error)
}

type SearchManager interface {
	Search(ctx context.Context, query string) (*models.SearchResult, error)
}

type TeamManager interface {
	Create(ctx context.Context, team models.Team) (*models.Team, error)
	Update(ctx context.Context, team models.Team) error
	Delete(ctx context.Context, id uuid.UUID) error
	Get(ctx context.Context, id uuid.UUID) (*models.Team, error)
	GetAll(ctx context.Context) ([]models.Team, error)
}

type ConfigManager interface {
	Init(ctx context.Context) error
}
