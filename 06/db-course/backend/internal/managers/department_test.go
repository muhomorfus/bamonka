package managers

import (
	"context"
	"errors"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/contextutils"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/mocks"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/models"
	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/ports"
	"github.com/gojuno/minimock/v3"
	"github.com/google/uuid"
	"reflect"
	"testing"
)

func TestDepartment_GetDepartments(t *testing.T) {
	mc := minimock.NewController(t)

	ds := []models.DepartmentShort{
		{ID: uuid.New(), Name: "test name 1"},
		{ID: uuid.New(), Name: "test name 2"},
	}

	type fields struct {
		repo ports.DepartmentRepository
	}
	type args struct {
		ctx context.Context
		id  uuid.UUID
	}
	tests := []struct {
		name    string
		fields  fields
		args    args
		want    []models.DepartmentShort
		wantErr bool
	}{
		{
			name: "valid breadcrumbs but not authorized",
			fields: fields{
				repo: mocks.NewDepartmentRepositoryMock(mc).BreadcrumbsMock.Return(ds, nil),
			},
			args: args{
				ctx: context.Background(),
			},
			want:    nil,
			wantErr: true,
		},
		{
			name: "valid breadcrumbs authorized",
			fields: fields{
				repo: mocks.NewDepartmentRepositoryMock(mc).BreadcrumbsMock.Return(ds, nil),
			},
			args: args{
				ctx: contextutils.SetEmployee(context.Background(), &models.Employee{}),
			},
			want:    ds,
			wantErr: false,
		},
		{
			name: "invalid get breadcrumbs",
			fields: fields{
				repo: mocks.NewDepartmentRepositoryMock(mc).BreadcrumbsMock.Return(nil, errors.New("error")),
			},
			args: args{
				ctx: contextutils.SetEmployee(context.Background(), &models.Employee{}),
			},
			want:    nil,
			wantErr: true,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			m := &Department{
				repo: tt.fields.repo,
			}
			got, err := m.GetDepartments(tt.args.ctx, tt.args.id)
			if (err != nil) != tt.wantErr {
				t.Errorf("GetDepartments() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if !reflect.DeepEqual(got, tt.want) {
				t.Errorf("GetDepartments() got = %v, want %v", got, tt.want)
			}
		})
	}
}
