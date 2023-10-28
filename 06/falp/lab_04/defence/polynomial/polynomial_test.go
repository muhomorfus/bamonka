package polynomial

import (
	"testing"
)

func TestSum(t *testing.T) {
	type args struct {
		p1 *Polynomial
		p2 *Polynomial
	}
	tests := []struct {
		name string
		args args
		want *Polynomial
	}{
		{
			name: "usual test",
			args: args{
				p1: New([]Monomial{
					{
						Coefficient: 1,
						Power:       1,
					},
				}),
				p2: New([]Monomial{
					{
						Coefficient: 1,
						Power:       0,
					},
				}),
			},
			want: New([]Monomial{
				{
					Coefficient: 1,
					Power:       1,
				},
				{
					Coefficient: 1,
					Power:       0,
				},
			}),
		},
		{
			name: "underlying coefficients",
			args: args{
				p1: New([]Monomial{
					{
						Coefficient: 3,
						Power:       1,
					},
				}),
				p2: New([]Monomial{
					{
						Coefficient: 2,
						Power:       1,
					},
					{
						Coefficient: 2,
						Power:       0,
					},
				}),
			},
			want: New([]Monomial{
				{
					Coefficient: 5,
					Power:       1,
				},
				{
					Coefficient: 2,
					Power:       0,
				},
			}),
		},
		{
			name: "empty value",
			args: args{
				p1: New([]Monomial{}),
				p2: New([]Monomial{
					{
						Coefficient: 2,
						Power:       1,
					},
					{
						Coefficient: 2,
						Power:       0,
					},
				}),
			},
			want: New([]Monomial{
				{
					Coefficient: 2,
					Power:       1,
				},
				{
					Coefficient: 2,
					Power:       0,
				},
			}),
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := Sum(tt.args.p1, tt.args.p2); !got.Equal(tt.want) {
				t.Errorf("Sum() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestSub(t *testing.T) {
	type args struct {
		p1 *Polynomial
		p2 *Polynomial
	}
	tests := []struct {
		name string
		args args
		want *Polynomial
	}{
		{
			name: "usual test",
			args: args{
				p1: New([]Monomial{
					{
						Coefficient: 1,
						Power:       1,
					},
				}),
				p2: New([]Monomial{
					{
						Coefficient: 1,
						Power:       0,
					},
				}),
			},
			want: New([]Monomial{
				{
					Coefficient: 1,
					Power:       1,
				},
				{
					Coefficient: -1,
					Power:       0,
				},
			}),
		},
		{
			name: "underlying coefficients",
			args: args{
				p1: New([]Monomial{
					{
						Coefficient: 3,
						Power:       1,
					},
				}),
				p2: New([]Monomial{
					{
						Coefficient: 2,
						Power:       1,
					},
					{
						Coefficient: 2,
						Power:       0,
					},
				}),
			},
			want: New([]Monomial{
				{
					Coefficient: 1,
					Power:       1,
				},
				{
					Coefficient: -2,
					Power:       0,
				},
			}),
		},
		{
			name: "empty value",
			args: args{
				p1: New([]Monomial{}),
				p2: New([]Monomial{
					{
						Coefficient: 2,
						Power:       1,
					},
					{
						Coefficient: 2,
						Power:       0,
					},
				}),
			},
			want: New([]Monomial{
				{
					Coefficient: -2,
					Power:       1,
				},
				{
					Coefficient: -2,
					Power:       0,
				},
			}),
		},
		{
			name: "result is nil",
			args: args{
				p1: New([]Monomial{
					{
						Coefficient: -2,
						Power:       1,
					},
					{
						Coefficient: 2,
						Power:       0,
					},
				}),
				p2: New([]Monomial{
					{
						Coefficient: -2,
						Power:       1,
					},
					{
						Coefficient: 2,
						Power:       0,
					},
				}),
			},
			want: New([]Monomial{}),
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := Sub(tt.args.p1, tt.args.p2); !got.Equal(tt.want) {
				t.Errorf("Sub() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestMul(t *testing.T) {
	type args struct {
		p1 *Polynomial
		p2 *Polynomial
	}
	tests := []struct {
		name string
		args args
		want *Polynomial
	}{
		{
			name: "usual",
			args: args{
				p1: New([]Monomial{
					{
						Coefficient: 2,
						Power:       3,
					},
					{
						Coefficient: 1,
						Power:       0,
					},
				}),
				p2: New([]Monomial{
					{
						Coefficient: 1,
						Power:       2,
					},
					{
						Coefficient: 7,
						Power:       1,
					},
				}),
			},
			want: New([]Monomial{
				{
					Coefficient: 2,
					Power:       5,
				},
				{
					Coefficient: 14,
					Power:       4,
				},
				{
					Coefficient: 1,
					Power:       2,
				},
				{
					Coefficient: 7,
					Power:       1,
				},
			}),
		},
		{
			name: "result is nil",
			args: args{
				p1: New([]Monomial{
					{
						Coefficient: -2,
						Power:       1,
					},
					{
						Coefficient: 2,
						Power:       0,
					},
				}),
				p2: New([]Monomial{}),
			},
			want: New([]Monomial{}),
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := Mul(tt.args.p1, tt.args.p2); !got.Equal(tt.want) {
				t.Errorf("Mul() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestDiv(t *testing.T) {
	type args struct {
		p1 *Polynomial
		p2 *Polynomial
	}
	tests := []struct {
		name    string
		args    args
		want    *Polynomial
		want1   *Polynomial
		wantErr bool
	}{
		{
			name: "result is nil",
			args: args{
				p1: New([]Monomial{}),
				p2: New([]Monomial{
					{
						Coefficient: -2,
						Power:       1,
					},
					{
						Coefficient: 2,
						Power:       0,
					},
				}),
			},
			want:    New([]Monomial{}),
			want1:   New([]Monomial{}),
			wantErr: false,
		},
		{
			name: "divide by nil",
			args: args{
				p1: New([]Monomial{
					{
						Coefficient: -2,
						Power:       1,
					},
					{
						Coefficient: 2,
						Power:       0,
					},
				}),
				p2: New([]Monomial{}),
			},
			want:    New([]Monomial{}),
			want1:   New([]Monomial{}),
			wantErr: true,
		},
		{
			name: "usual divide",
			args: args{
				p1: New([]Monomial{
					{
						Coefficient: 4,
						Power:       3,
					},
					{
						Coefficient: -5,
						Power:       2,
					},
					{
						Coefficient: 6,
						Power:       1,
					},
					{
						Coefficient: 9,
						Power:       0,
					},
				}),
				p2: New([]Monomial{
					{
						Coefficient: 1,
						Power:       2,
					},
					{
						Coefficient: -2,
						Power:       1,
					},
					{
						Coefficient: 3,
						Power:       0,
					},
				}),
			},
			want: New([]Monomial{
				{
					Coefficient: 4,
					Power:       1,
				},
				{
					Coefficient: 3,
					Power:       0,
				},
			}),
			want1:   New([]Monomial{}),
			wantErr: false,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, got1, err := Div(tt.args.p1, tt.args.p2)
			if (err != nil) != tt.wantErr {
				t.Errorf("Div() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if !tt.wantErr {
				if !got.Equal(tt.want) {
					t.Errorf("Div() got = %v, want %v", got, tt.want)
				}
				if !got1.Equal(tt.want1) {
					t.Errorf("Div() got1 = %v, want %v", got1, tt.want1)
				}
			}
		})
	}
}
