package gcache

import (
	"context"
	"errors"
	"fmt"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"github.com/bluele/gcache"
)

type Search struct {
	cache gcache.Cache
}

func NewSearch(cache gcache.Cache) *Search {
	return &Search{
		cache: cache,
	}
}

func (s *Search) Get(_ context.Context, query string) (*models.SearchResult, error) {
	v, err := s.cache.Get(query)
	switch {
	case errors.Is(err, gcache.KeyNotFoundError):
		return nil, models.ErrCacheMiss
	case err != nil:
		return nil, fmt.Errorf("get from cache: %w", err)
	}

	res, ok := v.(models.SearchResult)
	if !ok {
		return nil, fmt.Errorf("convert cached data: %w", models.ErrInvalidCacheData)
	}

	return &res, nil
}

func (s *Search) Set(_ context.Context, query string, res models.SearchResult) error {
	if err := s.cache.Set(query, res); err != nil {
		return fmt.Errorf("set to cache: %w", err)
	}

	return nil
}
