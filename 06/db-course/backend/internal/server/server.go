package server

import (
	"context"
	"net/http"
	"time"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/contextutils"
	"github.com/AlekSi/pointer"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/openapi"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/ports"
	"github.com/google/uuid"
	"github.com/samber/lo"
)

type Server struct {
	departmentMng ports.DepartmentManager
	teamMng       ports.TeamManager
	searchMng     ports.SearchManager
	profileMng    ports.ProfileManager
}

const dateFmt = "2006-01-02"

func NewServer(departmentMng ports.DepartmentManager, teamMng ports.TeamManager, searchMng ports.SearchManager, profileMng ports.ProfileManager) *Server {
	return &Server{departmentMng: departmentMng, teamMng: teamMng, searchMng: searchMng, profileMng: profileMng}
}

func (s *Server) CreateDepartment(ctx context.Context, req openapi.CreateDepartmentRequest) (openapi.ImplResponse, error) {
	var parentID *uuid.UUID
	if req.ParentId != nil {
		id, err := uuid.Parse(*req.ParentId)
		if err != nil {
			return toError(err, "Invalid parent department ID.")
		}
		parentID = pointer.To(id)
	}

	dep := models.Department{
		DepartmentShort: models.DepartmentShort{
			ID:   uuid.New(),
			Name: req.Name,
		},
		Description: req.Description,
		ParentID:    parentID,
	}
	res, err := s.departmentMng.Create(ctx, dep)
	if err != nil {
		return toError(err, "Cannot create department.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: toDepartment(*res),
	}, nil
}

func (s *Server) CreateEmployee(ctx context.Context, req openapi.CreateEmployeeRequest) (openapi.ImplResponse, error) {
	var bossID *uuid.UUID
	if req.BossId != nil {
		id, err := uuid.Parse(*req.BossId)
		if err != nil {
			return toError(err, "Cannot parse boss ID.")
		}
		bossID = pointer.To(id)
	}

	departmentID, err := uuid.Parse(req.DepartmentId)
	if err != nil {
		return toError(err, "Cannot parse department ID.")
	}

	teams := make([]models.TeamRole, len(req.Teams))
	for i, t := range req.Teams {
		id, err := uuid.Parse(req.Teams[i].Team.Id)
		if err != nil {
			return toError(err, "Cannot parse team ID.")
		}

		teams[i] = models.TeamRole{
			Team: models.Team{
				ID:          id,
				Name:        t.Team.Name,
				Description: t.Team.Description,
			},
			Role: t.Role,
			From: time.Now(),
		}
	}

	emp := models.Employee{
		ID:       uuid.New(),
		Name:     req.Name,
		Nickname: req.Nickname,
		Department: models.Department{
			DepartmentShort: models.DepartmentShort{
				ID: departmentID,
			},
		},
		Position: req.Position,
		Email:    req.Email,
		Phone:    req.Phone,
		Socials:  make(map[string]string),
		BossID:   bossID,
		Teams:    teams,
		Created:  time.Now().UTC(),
		Working:  true,
		Level:    models.LevelFromString(req.Level),
		Photo:    req.Photo,
	}

	res, err := s.profileMng.Create(ctx, emp, req.Password)
	if err != nil {
		return toError(err, "Cannot create employer.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: toEmployee(*res),
	}, nil
}

func (s *Server) CreateTeam(ctx context.Context, req openapi.CreateTeamRequest) (openapi.ImplResponse, error) {
	team := models.Team{
		ID:          uuid.New(),
		Name:        req.Name,
		Description: req.Description,
	}
	res, err := s.teamMng.Create(ctx, team)
	if err != nil {
		return toError(err, "Cannot create team.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: toTeam(*res),
	}, nil
}

func (s *Server) CreateVacation(ctx context.Context, req openapi.CreateVacationRequest) (openapi.ImplResponse, error) {
	emp, err := contextutils.GetEmployee(ctx)
	if err != nil {
		return toError(err, "Cannot get current employee.")
	}

	start, err := time.Parse(dateFmt, req.Start)
	if err != nil {
		return toError(err, "Invalid start time.")
	}

	end, err := time.Parse(dateFmt, req.End)
	if err != nil {
		return toError(err, "Invalid end time.")
	}

	vac := models.Vacation{
		ID:          uuid.New(),
		Start:       start,
		End:         end,
		Description: req.Description,
		Employee:    *emp,
	}

	res, err := s.profileMng.CreateVacation(ctx, vac)
	if err != nil {
		return toError(err, "Cannot create vacation.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: toVacation(*res),
	}, nil
}

func (s *Server) DeleteDepartment(ctx context.Context, id string) (openapi.ImplResponse, error) {
	departmentID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse department ID.")
	}
	if err := s.departmentMng.Delete(ctx, departmentID); err != nil {
		return toError(err, "Cannot delete department.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: nil,
	}, nil
}

func (s *Server) DeleteEmployee(ctx context.Context, id string) (openapi.ImplResponse, error) {
	employeeID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse employee ID.")
	}

	if err := s.profileMng.Delete(ctx, employeeID); err != nil {
		return toError(err, "Cannot delete employee.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: nil,
	}, nil
}

func (s *Server) DeleteTeam(ctx context.Context, id string) (openapi.ImplResponse, error) {
	teamID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse team ID.")
	}
	if err := s.teamMng.Delete(ctx, teamID); err != nil {
		return toError(err, "Cannot delete team.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: nil,
	}, nil
}

func (s *Server) FireEmployee(ctx context.Context, id string) (openapi.ImplResponse, error) {
	employeeID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse employee ID.")
	}

	if err := s.profileMng.Fire(ctx, employeeID); err != nil {
		return toError(err, "Cannot fire employee.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: nil,
	}, nil
}

func (s *Server) GetDepartment(ctx context.Context, id string) (openapi.ImplResponse, error) {
	departmentID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Invalid department ID.")
	}

	dep, err := s.departmentMng.Get(ctx, departmentID)
	if err != nil {
		return toError(err, "Cannot get department info.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: toDepartment(*dep),
	}, nil
}

func (s *Server) GetDepartmentBreadcrumbs(ctx context.Context, id string) (openapi.ImplResponse, error) {
	departmentID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse department ID.")
	}

	res, err := s.departmentMng.GetDepartments(ctx, departmentID)
	if err != nil {
		return toError(err, "Cannot get department list.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: openapi.GetDepartmentBreadcrumbsResponse{
			List: lo.Map(res, func(d models.DepartmentShort, _ int) openapi.DepartmentShort {
				return openapi.DepartmentShort{
					Id:   d.ID.String(),
					Name: d.Name,
				}
			}),
		},
	}, nil
}

func (s *Server) GetDepartmentList(ctx context.Context) (openapi.ImplResponse, error) {
	deps, err := s.departmentMng.GetAll(ctx)
	if err != nil {
		return toError(err, "Cannot get departments.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: openapi.GetDepartmentListResponse{
			List: lo.Map(deps, func(d models.Department, _ int) openapi.Department {
				return toDepartment(d)
			}),
		},
	}, nil
}

func (s *Server) GetEmployeeByID(ctx context.Context, id string) (openapi.ImplResponse, error) {
	employeeID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse employee ID.")
	}

	emp, err := s.profileMng.GetByID(ctx, employeeID)
	if err != nil {
		return toError(err, "Cannot get employee by ID.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: toEmployee(*emp),
	}, nil
}

func (s *Server) GetEmployeeByNickname(ctx context.Context, nickname string) (openapi.ImplResponse, error) {
	emp, err := s.profileMng.GetByNickname(ctx, nickname)
	if err != nil {
		return toError(err, "Cannot get employee by nickname.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: toEmployee(*emp),
	}, nil
}

func (s *Server) GetEmployeeList(ctx context.Context) (openapi.ImplResponse, error) {
	emps, err := s.profileMng.GetAll(ctx)
	if err != nil {
		return toError(err, "Cannot get employee list.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: openapi.GetEmployeeListResponse{
			List: lo.Map(emps, func(e models.Employee, _ int) openapi.Employee {
				return toEmployee(e)
			}),
		},
	}, nil
}

func (s *Server) GetTeamList(ctx context.Context) (openapi.ImplResponse, error) {
	teams, err := s.teamMng.GetAll(ctx)
	if err != nil {
		return toError(err, "Cannot get teams.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: openapi.GetTeamListResponse{
			List: lo.Map(teams, func(t models.Team, _ int) openapi.Team {
				return toTeam(t)
			}),
		},
	}, nil
}

func (s *Server) GetVacationList(ctx context.Context, id string) (openapi.ImplResponse, error) {
	employeeID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse employee ID.")
	}

	vacs, err := s.profileMng.GetNearestVacations(ctx, employeeID)
	if err != nil {
		return toError(err, "Cannot get nearest vacations.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: openapi.GetVacationListResponse{List: lo.Map(vacs, func(v models.Vacation, _ int) openapi.Vacation {
			return toVacation(v)
		})},
	}, nil
}

func (s *Server) Login(ctx context.Context, req openapi.LoginRequest) (openapi.ImplResponse, error) {
	token, err := s.profileMng.Login(ctx, req.Login, req.Password)
	if err != nil {
		return toError(err, "Cannot login to account.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: openapi.LoginResponse{Token: token},
	}, nil
}

func (s *Server) MeGet(ctx context.Context) (openapi.ImplResponse, error) {
	emp, err := contextutils.GetEmployee(ctx)
	if err != nil {
		return toError(err, "Cannot get current employee.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: toEmployee(*emp),
	}, nil
}

func (s *Server) Search(ctx context.Context, query string) (openapi.ImplResponse, error) {
	res, err := s.searchMng.Search(ctx, query)
	if err != nil {
		return toError(err, "Cannot search.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: openapi.SearchResponse{
			Employee: lo.Map(res.EmployeeList, func(e models.Employee, _ int) openapi.EmployeeShort {
				return openapi.EmployeeShort{
					Id:       e.ID.String(),
					Name:     e.Name,
					Nickname: e.Nickname,
					Working:  e.Working,
				}
			}),
			Teams: lo.Map(res.TeamList, func(t models.Team, _ int) openapi.TeamShort {
				return openapi.TeamShort{
					Id:   t.ID.String(),
					Name: t.Name,
				}
			}),
			Departments: lo.Map(res.DepartmentList, func(d models.Department, _ int) openapi.DepartmentShort {
				return openapi.DepartmentShort{
					Id:   d.ID.String(),
					Name: d.Name,
				}
			}),
		},
	}, nil
}

func (s *Server) Subscribe(ctx context.Context, id string) (openapi.ImplResponse, error) {
	authorID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse author ID.")
	}

	subscribed, err := s.profileMng.ChangeSubscription(ctx, authorID)
	if err != nil {
		return toError(err, "Cannot subscribe.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: openapi.SubscribeResponse{Subscribed: subscribed},
	}, nil
}

func (s *Server) IsSubscribed(ctx context.Context, id string) (openapi.ImplResponse, error) {
	authorID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse author ID.")
	}

	subscribed, err := s.profileMng.Subscribed(ctx, authorID)
	if err != nil {
		return toError(err, "Cannot subscribe.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: openapi.SubscribeResponse{Subscribed: subscribed},
	}, nil
}

func (s *Server) UpdateDepartment(ctx context.Context, id string, req openapi.UpdateDepartmentRequest) (openapi.ImplResponse, error) {
	departmentID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Invalid department ID.")
	}

	var parentID *uuid.UUID
	if req.ParentId != nil {
		id, err := uuid.Parse(*req.ParentId)
		if err != nil {
			return toError(err, "Invalid parent department ID.")
		}

		parentID = pointer.To(id)
	}

	dep := models.Department{
		DepartmentShort: models.DepartmentShort{
			ID:   departmentID,
			Name: req.Name,
		},
		Description: req.Description,
		ParentID:    parentID,
	}

	if err := s.departmentMng.Update(ctx, dep); err != nil {
		return toError(err, "Cannot update department.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: nil,
	}, nil
}

func (s *Server) UpdateEmployee(ctx context.Context, id string, employee openapi.Employee) (openapi.ImplResponse, error) {
	employeeID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse employee ID.")
	}

	var bossID *uuid.UUID
	if employee.BossId != nil {
		id, err := uuid.Parse(*employee.BossId)
		if err != nil {
			return toError(err, "Cannot parse boss ID.")
		}
		bossID = pointer.To(id)
	}

	var parentID *uuid.UUID
	if employee.Department.ParentId != nil {
		id, err := uuid.Parse(*employee.Department.ParentId)
		if err != nil {
			return toError(err, "Cannot parse department parent ID.")
		}
		parentID = pointer.To(id)
	}

	departmentID, err := uuid.Parse(employee.Department.Id)
	if err != nil {
		return toError(err, "Cannot parse department ID.")
	}

	teams := make([]models.TeamRole, len(employee.Teams))
	for i, t := range employee.Teams {
		id, err := uuid.Parse(employee.Teams[i].Team.Id)
		if err != nil {
			return toError(err, "Cannot parse team ID.")
		}

		from := time.Now().UTC()
		if t.From != "" {
			from, err = time.Parse(time.RFC3339, t.From)
			if err != nil {
				return toError(err, "Cannot parse time.")
			}
		}

		teams[i] = models.TeamRole{
			Team: models.Team{
				ID:          id,
				Name:        t.Team.Name,
				Description: t.Team.Description,
			},
			Role: t.Role,
			From: from,
		}
	}

	emp := models.Employee{
		ID:       employeeID,
		Name:     employee.Name,
		Nickname: employee.Nickname,
		Department: models.Department{
			DepartmentShort: models.DepartmentShort{
				ID:   departmentID,
				Name: employee.Department.Name,
			},
			Description: employee.Department.Description,
			ParentID:    parentID,
		},
		Position:    employee.Position,
		Email:       employee.Email,
		Phone:       employee.Phone,
		Socials:     employee.Socials,
		BossID:      bossID,
		Description: employee.Description,
		Teams:       teams,
		Level:       models.LevelFromString(employee.Level),
		Photo:       employee.Photo,
	}

	if err := s.profileMng.Update(ctx, emp); err != nil {
		return toError(err, "Cannot update employer.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: nil,
	}, nil
}

func (s *Server) UpdateTeam(ctx context.Context, id string, team openapi.Team) (openapi.ImplResponse, error) {
	teamID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse team ID.")
	}
	t := models.Team{
		ID:          teamID,
		Name:        team.Name,
		Description: team.Description,
	}
	if err := s.teamMng.Update(ctx, t); err != nil {
		return toError(err, "Cannot update team.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: nil,
	}, nil
}

func (s *Server) GetEmployeeByDepartment(ctx context.Context, id string) (openapi.ImplResponse, error) {
	departmentID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse department ID.")
	}

	emps, err := s.profileMng.GetByDepartment(ctx, departmentID)
	if err != nil {
		return toError(err, "Cannot get employee list for department.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: openapi.GetEmployeeListResponse{
			List: lo.Map(emps, func(e models.Employee, _ int) openapi.Employee {
				return toEmployee(e)
			}),
		},
	}, nil
}

func (s *Server) GetEmployeeByTeam(ctx context.Context, id string) (openapi.ImplResponse, error) {
	teamID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse team ID.")
	}

	emps, err := s.profileMng.GetByTeam(ctx, teamID)
	if err != nil {
		return toError(err, "Cannot get employee list for team.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: openapi.GetEmployeeListResponse{
			List: lo.Map(emps, func(e models.Employee, _ int) openapi.Employee {
				return toEmployee(e)
			}),
		},
	}, nil
}

func (s *Server) GetTeam(ctx context.Context, id string) (openapi.ImplResponse, error) {
	teamID, err := uuid.Parse(id)
	if err != nil {
		return toError(err, "Cannot parse team ID.")
	}

	team, err := s.teamMng.Get(ctx, teamID)
	if err != nil {
		return toError(err, "Cannot get team.")
	}

	return openapi.ImplResponse{
		Code: http.StatusOK,
		Body: toTeam(*team),
	}, nil
}
