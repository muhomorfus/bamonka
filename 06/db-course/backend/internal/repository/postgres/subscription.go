package postgres

import (
	"context"
	"fmt"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/roledb"

	"github.com/google/uuid"
)

type SubscriptionRepository struct {
	*roledb.RoleDB
}

func NewSubscriptionRepository(roleDB *roledb.RoleDB) *SubscriptionRepository {
	return &SubscriptionRepository{RoleDB: roleDB}
}

func (s *SubscriptionRepository) IsSubscribed(ctx context.Context, subscriber, author uuid.UUID) (bool, error) {
	var subscribed bool
	if err := s.DB(ctx).Table("subscription").Select("count(*)").
		Where("author_uuid = ?", author).
		Where("subscriber_uuid = ?", subscriber).
		Take(&subscribed).Error; err != nil {
		return false, fmt.Errorf("get subscription from db: %w", err)
	}

	return subscribed, nil
}

func (s *SubscriptionRepository) Subscribe(ctx context.Context, subscriber, author uuid.UUID) error {
	sub := Subscription{
		UUID:           uuid.New(),
		AuthorUUID:     author,
		SubscriberUUID: subscriber,
	}
	if err := s.DB(ctx).Table("subscription").Create(&sub).Error; err != nil {
		return fmt.Errorf("create subscription in db: %w", err)
	}

	return nil
}

func (s *SubscriptionRepository) Unsubscribe(ctx context.Context, subscriber, author uuid.UUID) error {
	if err := s.DB(ctx).Table("subscription").
		Where("author_uuid = ?", author).
		Where("subscriber_uuid = ?", subscriber).
		Delete(&Subscription{}).Error; err != nil {
		return fmt.Errorf("delete subscription from db: %w", err)
	}

	return nil
}
