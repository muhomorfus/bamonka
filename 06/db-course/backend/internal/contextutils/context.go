package contextutils

import (
	"context"
	"fmt"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
)

var keyEmployee struct{}

func SetEmployee(ctx context.Context, emp *models.Employee) context.Context {
	return context.WithValue(ctx, keyEmployee, emp)
}

func GetEmployee(ctx context.Context) (*models.Employee, error) {
	user := ctx.Value(keyEmployee)
	if user == nil {
		return nil, fmt.Errorf("user from context: %w", models.ErrNotAuthorized)
	}

	return user.(*models.Employee), nil
}

func IsAuthorized(ctx context.Context) bool {
	_, err := GetEmployee(ctx)
	return err == nil
}

func HasLevel(ctx context.Context, level models.Level) bool {
	emp, err := GetEmployee(ctx)
	if err != nil {
		return false
	}

	return emp.Level == level
}
