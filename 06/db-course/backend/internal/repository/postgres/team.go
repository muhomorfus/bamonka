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

type TeamRepository struct {
	*roledb.RoleDB
}

func NewTeamRepository(roleDB *roledb.RoleDB) *TeamRepository {
	return &TeamRepository{RoleDB: roleDB}
}

func (t *TeamRepository) Create(ctx context.Context, team models.Team) (*models.Team, error) {
	repoTeam := Team{
		UUID:        team.ID,
		Name:        team.Name,
		Description: team.Description,
	}

	if err := t.DB(ctx).Table("team").Create(&repoTeam).Error; err != nil {
		return nil, fmt.Errorf("create team in db: %w", err)
	}

	return &team, nil
}

func (t *TeamRepository) Update(ctx context.Context, team models.Team) error {
	repoTeam := Team{
		UUID:        team.ID,
		Name:        team.Name,
		Description: team.Description,
	}

	if err := t.DB(ctx).Table("team").Where("uuid = ?", repoTeam.UUID).Save(&repoTeam).Error; err != nil {
		return fmt.Errorf("save team in db: %w", err)
	}

	return nil
}

func (t *TeamRepository) Delete(ctx context.Context, id uuid.UUID) error {
	if err := t.DB(ctx).Table("team").Where("uuid = ?", id).Delete(&Team{}).Error; err != nil {
		return fmt.Errorf("delete team in db: %w", err)
	}

	return nil
}

func (t *TeamRepository) Get(ctx context.Context, id uuid.UUID) (*models.Team, error) {
	var repoTeam Team
	if err := t.DB(ctx).Table("team").Where("uuid = ?", id).Take(&repoTeam).Error; err != nil {
		return nil, fmt.Errorf("get team from db: %w", err)
	}

	return pointer.To(repoTeam.toModel()), nil
}

func (t *TeamRepository) ListSimilarByName(ctx context.Context, substr string) ([]models.Team, error) {
	var repoTeams []Team
	if err := t.DB(ctx).Table("team").Where("lower(name) like ?", "%"+substr+"%").Find(&repoTeams).Error; err != nil {
		return nil, fmt.Errorf("get teams from db: %w", err)
	}

	return lo.Map(repoTeams, func(e Team, _ int) models.Team {
		return e.toModel()
	}), nil
}

func (t *TeamRepository) GetAll(ctx context.Context) ([]models.Team, error) {
	var repoTeams []Team
	if err := t.DB(ctx).Table("team").Find(&repoTeams).Error; err != nil {
		return nil, fmt.Errorf("get teams from db: %w", err)
	}

	return lo.Map(repoTeams, func(e Team, _ int) models.Team {
		return e.toModel()
	}), nil
}
