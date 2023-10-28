package counter

import "sync"

// DummyCounter is counter for adding and subbing values.
// It contains max field for finding max value of all use cycle.
type DummyCounter struct {
	mu      sync.RWMutex
	current int
	max     int
}

// NewDummy creates a new DummyCounter.
func NewDummy() *DummyCounter {
	return new(DummyCounter)
}

// Add adds value to counter.
func (c *DummyCounter) Add(value int) {
	c.mu.Lock()
	defer c.mu.Unlock()

	c.current += value
	if c.current > c.max {
		c.max = c.current
	}
}

// Sub subs value from counter.
func (c *DummyCounter) Sub(value int) {
	c.mu.Lock()
	defer c.mu.Unlock()

	c.current -= value
	if c.current > c.max {
		c.max = c.current
	}
}

// Reset resets counter to zero values.
func (c *DummyCounter) Reset() {
	c.mu.Lock()
	defer c.mu.Unlock()

	c.current = 0
	c.max = 0
}

// Max returns max value of all counter work cycle.
func (c *DummyCounter) Max() int {
	c.mu.RLock()
	defer c.mu.RUnlock()

	return c.max
}

// Current returns max value of all counter work cycle.
func (c *DummyCounter) Current() int {
	c.mu.RLock()
	defer c.mu.RUnlock()

	return c.current
}
