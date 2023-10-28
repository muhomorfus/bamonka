package models

import "errors"

var (
	ErrDepartmentWithChildren = errors.New("department contains child departments")
	ErrShortQuery             = errors.New("query too short")
	ErrCacheMiss              = errors.New("cache miss")
	ErrLogin                  = errors.New("cant login")
	ErrNotAuthorized          = errors.New("not authorized")
	ErrForbidden              = errors.New("not enough permissions")
	ErrVacationsIntersects    = errors.New("vacations intersect")
	ErrInvalidCacheData       = errors.New("invalid cache data")
)
