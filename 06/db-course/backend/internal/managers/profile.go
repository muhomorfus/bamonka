package managers

import (
	"context"
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"time"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/contextutils"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/ports"
	"github.com/golang-jwt/jwt/v5"
	"github.com/google/uuid"
)

type Profile struct {
	empRepo ports.EmployeeRepository
	depRepo ports.DepartmentRepository
	subRepo ports.SubscriptionRepository
	vacRepo ports.VacationRepository

	expire time.Duration
	secret string
}

func NewProfile(er ports.EmployeeRepository, dr ports.DepartmentRepository, sr ports.SubscriptionRepository, vr ports.VacationRepository, expire time.Duration, secret string) *Profile {
	return &Profile{
		empRepo: er,
		depRepo: dr,
		subRepo: sr,
		vacRepo: vr,
		expire:  expire,
		secret:  secret,
	}
}

func (m *Profile) Create(ctx context.Context, emp models.Employee, password string) (*models.Employee, error) {
	switch {
	case contextutils.HasLevel(ctx, models.HR):
		if emp.Level != models.User {
			return nil, fmt.Errorf("check permissions: %w", models.ErrForbidden)
		}
	case contextutils.HasLevel(ctx, models.User):
		return nil, fmt.Errorf("check permissions: %w", models.ErrForbidden)
	}

	bytes := sha256.Sum256([]byte(password))
	emp.PasswordHash = hex.EncodeToString(bytes[:])

	e, err := m.empRepo.Create(ctx, emp)
	if err != nil {
		return nil, fmt.Errorf("create employee: %w", err)
	}

	return e, nil
}

func eqTeams(t1, t2 []models.TeamRole) bool {
	if len(t1) != len(t2) {
		return false
	}

	for i := range t1 {
		if t1[i].Role != t2[i].Role {
			return false
		}

		if t1[i].Team != t2[i].Team {
			return false
		}
	}

	return true
}

func canUpdateByEmployee(e1, e2 models.Employee) bool {
	return e1.Name == e2.Name && e1.Department.ID == e2.Department.ID &&
		e1.Position == e2.Position && e1.BossID == e2.BossID &&
		e1.Created == e2.Created && e1.Working == e2.Working &&
		e1.Level == e2.Level && eqTeams(e1.Teams, e2.Teams)
}

func canUpdateByHR(e1, e2 models.Employee) bool {
	return e1.Created == e2.Created && e1.Level == e2.Level
}

func (m *Profile) Update(ctx context.Context, emp models.Employee) error {
	user, err := contextutils.GetEmployee(ctx)
	if err != nil {
		return fmt.Errorf("auth: %w", err)
	}

	e, err := m.empRepo.GetByID(ctx, emp.ID)
	if err != nil {
		return fmt.Errorf("get employee: %w", err)
	}

	switch user.Level {
	case models.User:
		if !canUpdateByEmployee(emp, *e) {
			return fmt.Errorf("check update availablity by user: %w", models.ErrForbidden)
		}
	case models.HR:
		if !canUpdateByHR(emp, *e) {
			return fmt.Errorf("check update availablity by hr: %w", models.ErrForbidden)
		}
	}

	emp.PasswordHash = e.PasswordHash
	emp.Created = e.Created
	emp.Working = e.Working

	if err := m.empRepo.Update(ctx, emp); err != nil {
		return fmt.Errorf("update employee: %w", err)
	}

	return nil
}

func (m *Profile) Fire(ctx context.Context, id uuid.UUID) error {
	if !contextutils.IsAuthorized(ctx) || contextutils.HasLevel(ctx, models.User) {
		return fmt.Errorf("check permissions: %w", models.ErrForbidden)
	}

	if err := m.empRepo.SoftDelete(ctx, id); err != nil {
		return fmt.Errorf("soft delete employee: %w", err)
	}

	return nil
}

func (m *Profile) Delete(ctx context.Context, id uuid.UUID) error {
	if !contextutils.HasLevel(ctx, models.Admin) {
		return fmt.Errorf("check permissions: %w", models.ErrForbidden)
	}

	if err := m.empRepo.Delete(ctx, id); err != nil {
		return fmt.Errorf("delete employee: %w", err)
	}

	return nil
}

func (m *Profile) GetByID(ctx context.Context, id uuid.UUID) (*models.Employee, error) {
	if !contextutils.IsAuthorized(ctx) {
		return nil, fmt.Errorf("check permissions: %w", models.ErrNotAuthorized)
	}

	emp, err := m.empRepo.GetByID(ctx, id)
	if err != nil {
		return nil, fmt.Errorf("get employee: %w", err)
	}

	return emp, nil
}

func (m *Profile) GetByNickname(ctx context.Context, nick string) (*models.Employee, error) {
	if !contextutils.IsAuthorized(ctx) {
		return nil, fmt.Errorf("check permissions: %w", models.ErrNotAuthorized)
	}

	emp, err := m.empRepo.GetByNickname(ctx, nick)
	if err != nil {
		return nil, fmt.Errorf("get employee: %w", err)
	}

	return emp, nil
}

func (m *Profile) GetByJWT(ctx context.Context, token string) (*models.Employee, error) {
	var claims jwt.RegisteredClaims
	_, err := jwt.ParseWithClaims(token, &claims, func(_ *jwt.Token) (interface{}, error) {
		return []byte(m.secret), nil
	})
	if err != nil {
		return nil, fmt.Errorf("auth by token: %w (%s)", models.ErrLogin, err)
	}

	id, err := uuid.Parse(claims.ID)
	if err != nil {
		return nil, fmt.Errorf("parse uuid: %w", err)
	}

	emp, err := m.empRepo.GetByID(ctx, id)
	if err != nil {
		return nil, fmt.Errorf("get employee by id: %w", err)
	}

	return emp, nil
}

func (m *Profile) Login(ctx context.Context, nickname, password string) (string, error) {
	bytes := sha256.Sum256([]byte(password))
	hashed := hex.EncodeToString(bytes[:])

	emp, err := m.empRepo.GetByCredentials(ctx, nickname, hashed)
	if err != nil {
		return "", fmt.Errorf("get user by credentials: %w (%s)", models.ErrLogin, err)
	}

	claims := jwt.RegisteredClaims{
		ExpiresAt: jwt.NewNumericDate(time.Now().UTC().Add(m.expire)),
		IssuedAt:  jwt.NewNumericDate(time.Now().UTC()),
		ID:        emp.ID.String(),
	}

	token, err := jwt.NewWithClaims(jwt.SigningMethodHS256, claims).SignedString([]byte(m.secret))
	if err != nil {
		return "", fmt.Errorf("make token: %w", err)
	}

	return token, nil
}

func (m *Profile) ChangeSubscription(ctx context.Context, author uuid.UUID) (bool, error) {
	user, err := contextutils.GetEmployee(ctx)
	if err != nil {
		return false, fmt.Errorf("auth: %w", err)
	}

	subscribed, err := m.subRepo.IsSubscribed(ctx, user.ID, author)
	if err != nil {
		return false, fmt.Errorf("get subscription status: %w", err)
	}

	change := m.subRepo.Subscribe
	if subscribed {
		change = m.subRepo.Unsubscribe
	}

	if err := change(ctx, user.ID, author); err != nil {
		return false, fmt.Errorf("change subsription status to %v: %w", !subscribed, err)
	}

	return !subscribed, nil
}

func (m *Profile) CreateVacation(ctx context.Context, vac models.Vacation) (*models.Vacation, error) {
	user, err := contextutils.GetEmployee(ctx)
	if err != nil {
		return nil, fmt.Errorf("auth: %w", err)
	}

	vac.Employee = *user

	if vac.Start.After(vac.End) || vac.Start.Before(time.Now().UTC()) {
		return nil, fmt.Errorf("invalid vacation: %w", models.ErrVacationsIntersects)
	}

	vacations, err := m.vacRepo.GetList(ctx, user.ID)
	if err != nil {
		return nil, fmt.Errorf("get vacations: %w", err)
	}

	for _, v := range vacations {
		if !(vac.Start.After(v.End) || vac.End.Before(v.Start)) {
			return nil, fmt.Errorf("check vacation: %w", models.ErrVacationsIntersects)
		}
	}

	v, err := m.vacRepo.Create(ctx, vac)
	if err != nil {
		return nil, fmt.Errorf("create vacation: %w", err)
	}

	return v, nil
}

func (m *Profile) GetNearestVacations(ctx context.Context, id uuid.UUID) ([]models.Vacation, error) {
	if !contextutils.IsAuthorized(ctx) {
		return nil, fmt.Errorf("auth: %w", models.ErrNotAuthorized)
	}

	vacations, err := m.vacRepo.GetList(ctx, id)
	if err != nil {
		return nil, fmt.Errorf("get list vacations: %w", err)
	}

	return vacations, nil
}

func (m *Profile) GetAll(ctx context.Context) ([]models.Employee, error) {
	if !contextutils.IsAuthorized(ctx) {
		return nil, fmt.Errorf("auth: %w", models.ErrNotAuthorized)
	}

	e, err := m.empRepo.GetAll(ctx)
	if err != nil {
		return nil, fmt.Errorf("get list vacations: %w", err)
	}

	return e, nil
}

func (m *Profile) GetByDepartment(ctx context.Context, id uuid.UUID) ([]models.Employee, error) {
	if !contextutils.IsAuthorized(ctx) {
		return nil, fmt.Errorf("auth: %w", models.ErrNotAuthorized)
	}

	e, err := m.empRepo.GetByDepartment(ctx, id)
	if err != nil {
		return nil, fmt.Errorf("get list vacations by department: %w", err)
	}

	return e, nil
}

func (m *Profile) GetByTeam(ctx context.Context, id uuid.UUID) ([]models.Employee, error) {
	if !contextutils.IsAuthorized(ctx) {
		return nil, fmt.Errorf("auth: %w", models.ErrNotAuthorized)
	}

	e, err := m.empRepo.GetByTeam(ctx, id)
	if err != nil {
		return nil, fmt.Errorf("get list vacations by team: %w", err)
	}

	return e, nil
}

func (m *Profile) Subscribed(ctx context.Context, author uuid.UUID) (bool, error) {
	user, err := contextutils.GetEmployee(ctx)
	if err != nil {
		return false, fmt.Errorf("auth: %w", err)
	}

	subscribed, err := m.subRepo.IsSubscribed(ctx, user.ID, author)
	if err != nil {
		return false, fmt.Errorf("get subscription status: %w", err)
	}

	return subscribed, nil
}
