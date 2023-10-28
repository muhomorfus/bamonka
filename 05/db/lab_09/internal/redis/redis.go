package redis

import (
	"context"
	"encoding/json"
	redisLib "github.com/go-redis/redis/v9"
	"lab_09/internal/models"
	"time"
)

const expire = 5 * time.Minute

type Redis struct {
	c redisLib.UniversalClient
}

func New(c redisLib.UniversalClient) *Redis {
	return &Redis{
		c: c,
	}
}

func (r *Redis) Get(key string) ([]models.Staff, error) {
	raw, err := r.c.Get(context.Background(), key).Bytes()
	if err != nil {
		return nil, err
	}

	var staff []models.Staff
	if err := json.Unmarshal(raw, &staff); err != nil {
		return nil, err
	}

	return staff, nil
}

func (r *Redis) Set(key string, staff []models.Staff) error {
	raw, err := json.Marshal(staff)
	if err != nil {
		return err
	}

	if _, err := r.c.Set(context.Background(), key, raw, expire).Result(); err != nil {
		return err
	}

	return nil
}

func (r *Redis) Remove(key string) error {
	return r.c.Del(context.Background(), key).Err()
}
