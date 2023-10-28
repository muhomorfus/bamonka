package app

import (
	"context"
	"fmt"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/contextutils"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"github.com/samber/lo"
	"k8s.io/apimachinery/pkg/util/sets"
	"math/rand"
	"sort"
	"testing"
	"time"
)

func init() {
	testing.Init()
}

const (
	iterations = 1000
	numQueries = 10
	queryLen   = 5
)

func (a *App) getQueries(ctx context.Context) ([]string, error) {
	a.logger.Infow("getting departments")
	deps, err := a.departmentRepo.GetAll(ctx)
	if err != nil {
		return nil, fmt.Errorf("get all departments: %w", err)
	}

	a.logger.Infow("getting teams")
	teams, err := a.teamRepo.GetAll(ctx)
	if err != nil {
		return nil, fmt.Errorf("get all teams: %w", err)
	}

	a.logger.Infow("getting employee")
	emps, err := a.employeeRepo.GetAll(ctx)
	if err != nil {
		return nil, fmt.Errorf("get all employee: %w", err)
	}

	depsQueries := lo.Map(deps, func(e models.Department, _ int) string {
		return e.Name[2 : 2+queryLen]
	})

	teamsQueries := lo.Map(teams, func(e models.Team, _ int) string {
		return e.Name[2 : 2+queryLen]
	})

	empsQueries := lo.Map(emps, func(e models.Employee, _ int) string {
		return e.Name[2 : 2+queryLen]
	})

	querySet := sets.New[string](depsQueries...)
	querySet = querySet.Insert(teamsQueries...)
	querySet = querySet.Insert(empsQueries...)

	a.logger.Infow("got query", "len", querySet.Len())

	list := querySet.UnsortedList()
	sort.Slice(list, func(i, j int) bool {
		return list[i] < list[j]
	})
	if len(list) > numQueries {
		list = list[:numQueries]
	}

	return list, nil
}

func (a *App) Benchmark(ctx context.Context) error {
	a.logger.Infow("starting generate application")

	rand.Seed(time.Now().UnixNano())
	ctx = contextutils.SetEmployee(ctx, &models.Employee{Level: models.Admin})

	queries, err := a.getQueries(ctx)
	if err != nil {
		return fmt.Errorf("get queries: %w", err)
	}

	start := time.Now()
	for i := 0; i < iterations; i++ {
		_, _ = a.searchMng.Search(ctx, queries[i%numQueries])
	}
	duration := time.Since(start)

	fmt.Printf("Result: %d\n", duration.Microseconds()/iterations)

	return nil
}
