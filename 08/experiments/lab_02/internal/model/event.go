package model

import (
	"errors"
	"fmt"

	"github.com/emirpasic/gods/queues/priorityqueue"
)

var (
	ErrCantGetEvent = errors.New("cant get event from queue")
)

type EventModel struct {
	*BaseModel
}

func NewEventModel(base *BaseModel) *EventModel {
	return &EventModel{
		BaseModel: base,
	}
}

func (m *EventModel) compute(limit int) (float64, error) {
	avgWaitTime := 0.0

	q := newQueue(limit)
	events := priorityqueue.NewWith(byTimestamp)

	generateTime := m.generatorTimer.Rand()
	events.Enqueue(event{
		eventType: eventGenerated,
		timestamp: generateTime,
	})

	generate := true

	processedMessages, generatedMessages := 0, 0
	for generate || !events.Empty() {
		if generate && processedMessages >= limit {
			generate = false
		}

		rawEvent, ok := events.Dequeue()
		if !ok {
			return 0.0, ErrCantGetEvent
		}
		e := rawEvent.(event)

		switch e.eventType {
		case eventGenerated:
			if !generate {
				generatedMessages++
				continue
			}
			// Настало время генерировать новое сообщение в очередь.
			wasEmpty := q.empty()
			q.push(e.timestamp)
			generatedMessages++

			generatePeriod := m.generatorTimer.Rand()

			events.Enqueue(event{
				eventType: eventGenerated,
				timestamp: e.timestamp + generatePeriod,
			})

			if !wasEmpty {
				continue
			}
		case eventProcessed:
			if q.empty() {
				continue
			}

			avgWaitTime += q.pop(e.timestamp)
			processedMessages++
		}

		if !q.empty() {
			processPeriod := m.processorTimer.Rand()

			events.Enqueue(event{
				eventType: eventProcessed,
				timestamp: e.timestamp + processPeriod,
			})
		}
	}

	avgWaitTime /= float64(processedMessages)

	return avgWaitTime, nil
}

func (m *EventModel) Compute(limit int) (float64, error) {
	const retries = 2

	averageTime := 0.0

	for i := 0; i < retries; i++ {
		res, err := m.compute(limit)
		if err != nil {
			return 0.0, fmt.Errorf("compute model: %w", err)
		}

		averageTime += res
	}

	averageTime /= float64(retries)

	return averageTime, nil
}
