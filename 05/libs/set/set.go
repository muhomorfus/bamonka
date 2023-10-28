package set

import "sync"

type empty struct{}

// Set is base set container based on go maps.
type Set[T comparable] struct {
	mu   sync.RWMutex
	data map[T]empty
}

func New[T comparable]() *Set[T] {
	return &Set[T]{
		data: make(map[T]empty),
	}
}

// Add adds value to set. If set already contains this value, no duplicates is added.
func (s *Set[T]) Add(value T) {
	s.mu.Lock()
	defer s.mu.Unlock()

	s.data[value] = empty{}
}

// Delete deletes value from set.
func (s *Set[T]) Delete(value T) {
	s.mu.Lock()
	defer s.mu.Unlock()

	delete(s.data, value)
}

// Contains checks set to contain specified value.
func (s *Set[T]) Contains(value T) bool {
	s.mu.RLock()
	defer s.mu.RUnlock()

	_, ok := s.data[value]
	return ok
}
