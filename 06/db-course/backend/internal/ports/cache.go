package ports

import (
	"context"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
)

type SearchCache interface {
	Get(ctx context.Context, query string) (*models.SearchResult, error)
	Set(ctx context.Context, query string, res models.SearchResult) error
}
