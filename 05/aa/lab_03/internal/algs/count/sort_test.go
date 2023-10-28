package count

import (
	"reflect"
	"testing"
)

func TestCount_Sort(t *testing.T) {
	type args struct {
		a []int
	}
	tests := []struct {
		name string
		args args
		want []int
	}{
		{
			name: "Случайный массив",
			args: args{
				a: []int{5, 6, 2, 6, 1, 7},
			},
			want: []int{1, 2, 5, 6, 6, 7},
		},
		{
			name: "Отсортированный массив",
			args: args{
				a: []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
			},
			want: []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
		},
		{
			name: "Отсортированный в обратном порядке массив",
			args: args{
				a: []int{10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
			},
			want: []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
		},
		{
			name: "Массив, состоящий из одних и тех же элементов",
			args: args{
				a: []int{1, 1, 1, 1, 1, 1},
			},
			want: []int{1, 1, 1, 1, 1, 1},
		},
		{
			name: "Массив, состоящий из одного элемента",
			args: args{
				a: []int{1},
			},
			want: []int{1},
		},
		{
			name: "Пустой массив",
			args: args{
				a: []int{},
			},
			want: []int{},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			m := &Count{}
			if got := m.Sort(tt.args.a); !reflect.DeepEqual(got, tt.want) {
				t.Errorf("Sort() = %v, want %v", got, tt.want)
			}
		})
	}
}
