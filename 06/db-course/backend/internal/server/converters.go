package server

import (
	"errors"
	"net/http"
	"time"

	"github.com/AlekSi/pointer"
	"github.com/samber/lo"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/openapi"
)

func toError(err error, defaultComment string) (openapi.ImplResponse, error) {
	switch {
	case errors.Is(err, models.ErrForbidden):
		return openapi.ImplResponse{
			Code: http.StatusForbidden,
			Body: openapi.Error{
				Error:   err.Error(),
				Comment: "You have not enough privileges to do that.",
			},
		}, nil
	case errors.Is(err, models.ErrNotAuthorized):
		return openapi.ImplResponse{
			Code: http.StatusUnauthorized,
			Body: openapi.Error{
				Error:   err.Error(),
				Comment: "You are not authorized.",
			},
		}, nil
	case errors.Is(err, models.ErrShortQuery):
		return openapi.ImplResponse{
			Code: http.StatusBadRequest,
			Body: openapi.Error{
				Error:   err.Error(),
				Comment: "Search query must me at least 3 symbols.",
			},
		}, nil
	case errors.Is(err, models.ErrLogin):
		return openapi.ImplResponse{
			Code: http.StatusBadRequest,
			Body: openapi.Error{
				Error:   err.Error(),
				Comment: "Login/password not match.",
			},
		}, nil
	case errors.Is(err, models.ErrDepartmentWithChildren):
		return openapi.ImplResponse{
			Code: http.StatusBadRequest,
			Body: openapi.Error{
				Error:   err.Error(),
				Comment: "Cannot delete department with child departments.",
			},
		}, nil
	case errors.Is(err, models.ErrVacationsIntersects):
		return openapi.ImplResponse{
			Code: http.StatusConflict,
			Body: openapi.Error{
				Error:   err.Error(),
				Comment: "Suggested vacation intersects existing vacation.",
			},
		}, nil
	default:
		return openapi.ImplResponse{
			Code: http.StatusInternalServerError,
			Body: openapi.Error{
				Error:   err.Error(),
				Comment: defaultComment,
			},
		}, nil
	}
}

func toDepartment(d models.Department) openapi.Department {
	var parentID *string
	if d.ParentID != nil {
		parentID = pointer.To(d.ParentID.String())
	}

	return openapi.Department{
		Id:          d.ID.String(),
		Name:        d.Name,
		Description: d.Description,
		ParentId:    parentID,
	}
}

func toTeam(t models.Team) openapi.Team {
	return openapi.Team{
		Id:          t.ID.String(),
		Name:        t.Name,
		Description: t.Description,
	}
}

func toEmployee(e models.Employee) openapi.Employee {
	var parentID *string
	if e.Department.ParentID != nil {
		parentID = pointer.To(e.Department.ParentID.String())
	}

	var bossID *string
	if e.BossID != nil {
		bossID = pointer.To(e.BossID.String())
	}

	return openapi.Employee{
		Id:       e.ID.String(),
		Name:     e.Name,
		Nickname: e.Nickname,
		Department: openapi.Department{
			Id:          e.Department.ID.String(),
			Name:        e.Department.Name,
			Description: e.Department.Description,
			ParentId:    parentID,
		},
		Position:    e.Position,
		Email:       e.Email,
		Phone:       e.Phone,
		Socials:     e.Socials,
		BossId:      bossID,
		Description: e.Description,
		Photo:       e.Photo,
		Level:       e.Level.String(),
		Teams: lo.Map(e.Teams, func(t models.TeamRole, _ int) openapi.TeamRole {
			return openapi.TeamRole{
				Team: toTeam(t.Team),
				Role: t.Role,
				From: t.From.Format(time.RFC3339),
			}
		}),
		Created: e.Created.Format(time.RFC3339),
		Working: e.Working,
	}
}

func toVacation(v models.Vacation) openapi.Vacation {
	return openapi.Vacation{
		Start:       v.Start.Format(time.RFC3339),
		End:         v.End.Format(time.RFC3339),
		Description: v.Description,
		Id:          v.ID.String(),
	}
}
