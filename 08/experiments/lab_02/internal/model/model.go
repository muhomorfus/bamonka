package model

import (
	"github.com/emirpasic/gods/utils"
)

type Result struct {
	QueueLen          int
	GenerateIntencity float64
	ProcessIntencity  float64
	Load              float64
	TheoryLoad        float64
	AverageTime       float64
}

type queue struct {
	cur   int
	max   int
	pos   int
	elems []float64
}

func newQueue(limit int) *queue {
	return &queue{elems: make([]float64, 0, limit)}
}

func (q *queue) push(timestamp float64) {
	q.elems = append(q.elems, timestamp)

	q.cur++
	if q.cur > q.max {
		q.max = q.cur
	}
}

func (q *queue) pop(timestamp float64) float64 {
	if q.cur > 0 {
		q.cur--

		q.pos++
		return timestamp - q.elems[q.pos-1]
	}

	return 0
}

func (q *queue) empty() bool {
	return q.cur == 0
}

func (q *queue) maxLen() int {
	return q.max
}

type eventType int

const (
	eventGenerated eventType = iota + 1
	eventProcessed
)

type event struct {
	eventType eventType
	timestamp float64
}

func byTimestamp(a, b interface{}) int {
	return utils.Float64Comparator(a.(event).timestamp, b.(event).timestamp)
}
