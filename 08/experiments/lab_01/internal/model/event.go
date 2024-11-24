package model

import (
	"errors"
	"fmt"
	"math"
	mathRand "math/rand"
	"os"

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

func (m *EventModel) compute(limit, percent int) (*Result, error) {
	avgWaitTime := 0.0

	q := newQueue(limit)
	events := priorityqueue.NewWith(byTimestamp)

	minGenerateTime := math.MaxFloat64
	for i := range m.generatorTimers {
		generateTime, err := m.generatorTimers[i].Rand()
		if err != nil {
			return nil, err
		}

		if generateTime < minGenerateTime {
			minGenerateTime = generateTime
		}

		events.Enqueue(event{
			eventType: eventGenerated,
			n:         i,
			timestamp: generateTime,
		})
	}

	start := minGenerateTime
	end := 0.0
	endGenerate := 0.0
	generate := true

	processedMessages, returnedMessages, generatedMessages := 0, 0, 0
	for generate || !events.Empty() {
		if generate && processedMessages >= limit {
			generate = false
			endGenerate = end
		}

		rawEvent, ok := events.Dequeue()
		if !ok {
			return nil, ErrCantGetEvent
		}
		e := rawEvent.(event)
		end = e.timestamp

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

			generatePeriod, err := m.generatorTimers[e.n].Rand()
			if err != nil {
				return nil, err
			}

			events.Enqueue(event{
				eventType: eventGenerated,
				n:         e.n,
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

			if mathRand.Intn(100) < percent {
				q.push(e.timestamp)
				returnedMessages++
			}
		}

		if !q.empty() {
			processPeriod, err := m.processorTimer.Rand()
			if err != nil {
				return nil, err
			}

			events.Enqueue(event{
				eventType: eventProcessed,
				timestamp: e.timestamp + processPeriod,
			})
		}
	}

	interval := end - start
	avgWaitTime /= float64(processedMessages)
	avgGenerateIntencity := float64(generatedMessages) / (endGenerate - start)
	avgProcessIntencity := float64(processedMessages) / interval
	load := avgGenerateIntencity / avgProcessIntencity

	return &Result{
		QueueLen:          q.maxLen(),
		ProcessIntencity:  avgProcessIntencity,
		GenerateIntencity: avgGenerateIntencity,
		Load:              load,
		AverageTime:       avgWaitTime,
	}, nil
}

func (m *EventModel) Compute(limit, retries int) (*Result, error) {
	fileCome, err := os.OpenFile("data/wait_come.txt", os.O_RDWR|os.O_APPEND|os.O_CREATE, 0755)
	if err != nil {
		return nil, fmt.Errorf("open file: %w", err)
	}

	fileProc, err := os.OpenFile("data/wait_proc.txt", os.O_RDWR|os.O_APPEND|os.O_CREATE, 0755)
	if err != nil {
		return nil, fmt.Errorf("open file: %w", err)
	}

	fileLoad, err := os.OpenFile("data/wait_load.txt", os.O_RDWR|os.O_APPEND|os.O_CREATE, 0755)
	if err != nil {
		return nil, fmt.Errorf("open file: %w", err)
	}

	queueLen := 0
	processIntencity := 0.0
	generateIntencity := 0.0
	load := 0.0
	averageTime := 0.0

	for i := 0; i < retries; i++ {
		res, err := m.compute(limit, 0.0)
		if err != nil {
			return nil, fmt.Errorf("compute model: %w", err)
		}

		queueLen += res.QueueLen
		processIntencity += res.ProcessIntencity
		generateIntencity += res.GenerateIntencity
		load += res.Load
		averageTime += res.AverageTime
	}

	queueLen /= retries
	processIntencity /= float64(retries)
	generateIntencity /= float64(retries)
	load /= float64(retries)
	averageTime /= float64(retries)

	_, err = fileCome.Write([]byte(fmt.Sprintf("%f %f\n", m.GenerateIntensity(), averageTime)))
	if err != nil {
		return nil, fmt.Errorf("write to file: %w", err)
	}

	_, err = fileProc.Write([]byte(fmt.Sprintf("%f %f\n", m.ProcessIntensity(), averageTime)))
	if err != nil {
		return nil, fmt.Errorf("write to file: %w", err)
	}

	_, err = fileLoad.Write([]byte(fmt.Sprintf("%f %f\n", m.Load(), averageTime)))
	if err != nil {
		return nil, fmt.Errorf("write to file: %w", err)
	}

	return &Result{
		QueueLen:          queueLen,
		ProcessIntencity:  processIntencity,
		GenerateIntencity: generateIntencity,
		Load:              load,
		TheoryLoad:        m.Load(),
		AverageTime:       averageTime,
	}, nil
}
