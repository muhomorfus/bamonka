package roledb

import (
	"context"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/contextutils"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"gorm.io/gorm"
)

type RoleDB struct {
	dbs map[models.Level]*gorm.DB
}

func New() *RoleDB {
	r := new(RoleDB)
	r.dbs = make(map[models.Level]*gorm.DB)

	return r
}

func (r *RoleDB) Append(level models.Level, db *gorm.DB) {
	r.dbs[level] = db
}

func (r *RoleDB) DB(ctx context.Context) *gorm.DB {
	emp, err := contextutils.GetEmployee(ctx)
	if err != nil {
		return r.dbs[models.User]
	}

	return r.dbs[emp.Level].WithContext(ctx)
}
