package counter

import (
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestDummyCounter_Add(t *testing.T) {
	d := NewDummy()

	d.Add(5)
	d.Add(3)

	assert.Equalf(t, d.current, 8, "current value must be 8")
	assert.Equalf(t, d.max, 8, "max value must be 8")
}

func TestDummyCounter_Sub(t *testing.T) {
	d := NewDummy()

	d.Add(10)
	d.Sub(3)

	assert.Equalf(t, d.current, 7, "current value must be 7")
	assert.Equalf(t, d.max, 10, "max value must be 10")
}

func TestDummyCounter_Reset(t *testing.T) {
	d := NewDummy()

	d.Add(10)
	d.Add(5)
	d.Sub(3)
	d.Add(2)
	d.Sub(7)
	d.Reset()

	assert.Equalf(t, d.current, 0, "current value must be 0")
	assert.Equalf(t, d.max, 0, "max value must be 0")
}
