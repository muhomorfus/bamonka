package managers

import (
	"context"
	"errors"
	"fmt"
	"go.uber.org/zap"
	"strings"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/contextutils"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/ports"
)

type Search struct {
	cache    ports.SearchCache
	teamRepo ports.TeamRepository
	depRepo  ports.DepartmentRepository
	empRepo  ports.EmployeeRepository
	logger   *zap.SugaredLogger
}

func NewSearch(cache ports.SearchCache, tr ports.TeamRepository, dr ports.DepartmentRepository, er ports.EmployeeRepository, logger *zap.SugaredLogger) *Search {
	return &Search{
		cache:    cache,
		teamRepo: tr,
		depRepo:  dr,
		empRepo:  er,
		logger:   logger,
	}
}

func (m *Search) Search(ctx context.Context, query string) (*models.SearchResult, error) {
	if !contextutils.IsAuthorized(ctx) {
		return nil, fmt.Errorf("check permissions: %w", models.ErrNotAuthorized)
	}

	query = strings.ToLower(strings.TrimSpace(query))

	if len(query) < 3 {
		return nil, fmt.Errorf("validate query: %w", models.ErrShortQuery)
	}

	res, err := m.cache.Get(ctx, query)
	switch {
	case err == nil:
		return res, nil
	case errors.Is(err, models.ErrCacheMiss):
		m.logger.Warnw("cache miss", "query", query)
	default:
		m.logger.Errorw("cannot get from cache", "error", err)
	}

	teams, err := m.teamRepo.ListSimilarByName(ctx, query)
	if err != nil {
		return nil, fmt.Errorf("get teams: %w", err)
	}

	deps, err := m.depRepo.ListSimilarByName(ctx, query)
	if err != nil {
		return nil, fmt.Errorf("get departments: %w", err)
	}

	empName, err := m.empRepo.ListSimilarByName(ctx, query)
	if err != nil {
		return nil, fmt.Errorf("get employee: %w", err)
	}

	empNick, err := m.empRepo.ListSimilarByNickname(ctx, query)
	if err != nil {
		return nil, fmt.Errorf("get employee by nickname: %w", err)
	}

	res = &models.SearchResult{
		EmployeeList:   append(empNick, empName...),
		TeamList:       teams,
		DepartmentList: deps,
	}

	if err := m.cache.Set(ctx, query, *res); err != nil {
		return nil, fmt.Errorf("set search result: %w", err)
	}

	return res, nil
}
