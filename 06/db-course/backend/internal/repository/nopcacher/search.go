package nopcacher

import (
	"context"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
)

type Search struct {
}

func NewSearch() *Search {
	return &Search{}
}

func (s *Search) Get(_ context.Context, query string) (*models.SearchResult, error) {
	return nil, models.ErrCacheMiss
}

func (s *Search) Set(_ context.Context, query string, res models.SearchResult) error {
	return nil
}
