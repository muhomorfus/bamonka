package ports

type Coder interface {
	Marshal(v any) (string, error)
	Unmarshal(data string, v any) error
}
