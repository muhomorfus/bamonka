package set

import (
	"fmt"
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestSet_Add(t *testing.T) {
	s := New[int]()

	elements := []int{1, 1, 1, 2, 2, 3, 6, 1, 2, 3}

	for _, v := range elements {
		s.Add(v)
	}

	for _, v := range elements {
		_, ok := s.data[v]
		assert.Truef(t, ok, "%d must be in set", v)
	}
}

func TestSet_Delete(t *testing.T) {
	s := New[int]()

	elements := []int{1, 1, 1, 2, 2, 3, 6, 1, 2, 3}

	for _, v := range elements {
		s.Add(v)
	}

	v := 1
	s.Delete(v)
	_, ok := s.data[v]
	assert.Falsef(t, ok, "%d must not be in set", v)
}

func TestSet_Contains(t *testing.T) {
	s := New[int]()

	elements := []int{1, 1, 1, 2, 2, 3, 6, 1, 2, 3}

	for _, v := range elements {
		s.Add(v)
	}

	for _, v := range elements {
		contains := s.Contains(v)
		assert.Truef(t, contains, "%d must be in set", v)
	}

	notContains := []int{50, 4, 20, 10, 12}

	for _, v := range notContains {
		contains := s.Contains(v)
		assert.Falsef(t, contains, "%d must not be in set", v)
	}
}

func ExampleSet_Add() {
	s := New[int]()

	s.Add(1)
	s.Add(2)
}

func ExampleSet_Delete() {
	s := New[int]()

	s.Add(1)
	s.Add(2)

	s.Delete(2)
}

func ExampleSet_Contains() {
	s := New[int]()

	s.Add(1)
	s.Add(2)

	s.Delete(2)

	s.Add(3)

	fmt.Println(s.Contains(1))
	fmt.Println(s.Contains(2))
	fmt.Println(s.Contains(3))
	fmt.Println(s.Contains(4))
	fmt.Println(s.Contains(20))

	// Output:
	// true
	// false
	// true
	// false
	// false
}
