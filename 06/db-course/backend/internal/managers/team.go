package managers

import (
	"context"
	"fmt"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/contextutils"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/ports"
	"github.com/google/uuid"
)

type Team struct {
	repo ports.TeamRepository
}

func NewTeam(repo ports.TeamRepository) *Team {
	return &Team{
		repo: repo,
	}
}

func (m *Team) Create(ctx context.Context, team models.Team) (*models.Team, error) {
	if !contextutils.HasLevel(ctx, models.Admin) {
		return nil, fmt.Errorf("check permissions: %w", models.ErrForbidden)
	}

	t, err := m.repo.Create(ctx, team)
	if err != nil {
		return nil, fmt.Errorf("create team: %w", err)
	}

	return t, nil
}

func (m *Team) Update(ctx context.Context, team models.Team) error {
	if !contextutils.HasLevel(ctx, models.Admin) {
		return fmt.Errorf("check permissions: %w", models.ErrForbidden)
	}

	if err := m.repo.Update(ctx, team); err != nil {
		return fmt.Errorf("update team: %w", err)
	}

	return nil
}

func (m *Team) Delete(ctx context.Context, id uuid.UUID) error {
	if !contextutils.HasLevel(ctx, models.Admin) {
		return fmt.Errorf("check permissions: %w", models.ErrForbidden)
	}

	if err := m.repo.Delete(ctx, id); err != nil {
		return fmt.Errorf("delete team: %w", err)
	}

	return nil
}

func (m *Team) Get(ctx context.Context, id uuid.UUID) (*models.Team, error) {
	if !contextutils.IsAuthorized(ctx) {
		return nil, fmt.Errorf("check permissions: %w", models.ErrNotAuthorized)
	}

	team, err := m.repo.Get(ctx, id)
	if err != nil {
		return nil, fmt.Errorf("get team: %w", err)
	}

	return team, nil
}

func (m *Team) GetAll(ctx context.Context) ([]models.Team, error) {
	if !contextutils.IsAuthorized(ctx) {
		return nil, fmt.Errorf("check permissions: %w", models.ErrNotAuthorized)
	}

	team, err := m.repo.GetAll(ctx)
	if err != nil {
		return nil, fmt.Errorf("get team: %w", err)
	}

	return team, nil
}
