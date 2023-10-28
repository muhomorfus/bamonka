package redis

import (
	"context"
	"errors"
	"fmt"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/ports"
	"github.com/redis/go-redis/v9"
	"time"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
)

type Search struct {
	cache *redis.Client
	coder ports.Coder
	ttl   time.Duration
}

func NewSearch(cache *redis.Client, coder ports.Coder, ttl time.Duration) *Search {
	return &Search{
		cache: cache,
		coder: coder,
		ttl:   ttl,
	}
}

func (s *Search) Get(ctx context.Context, query string) (*models.SearchResult, error) {
	val, err := s.cache.Get(ctx, query).Result()
	switch {
	case errors.Is(err, redis.Nil):
		return nil, models.ErrCacheMiss
	case err != nil:
		return nil, fmt.Errorf("get from cache: %w", err)
	}

	var res models.SearchResult
	if err := s.coder.Unmarshal(val, &res); err != nil {
		return nil, fmt.Errorf("unmarshal: %w", err)
	}

	return &res, nil
}

func (s *Search) Set(ctx context.Context, query string, res models.SearchResult) error {
	val, err := s.coder.Marshal(res)
	if err != nil {
		return fmt.Errorf("marshal: %w", err)
	}

	if err := s.cache.Set(ctx, query, val, s.ttl).Err(); err != nil {
		return fmt.Errorf("set to cache: %w", err)
	}

	return nil
}
