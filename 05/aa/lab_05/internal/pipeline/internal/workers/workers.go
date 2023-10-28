package workers

import (
	"lab_05/internal/pipeline/internal/task"
	"lab_05/internal/text"
	"time"
)

func Normalize(in <-chan task.Task, out chan<- task.Task) {
	for t := range in {
		t.StartTime1 = time.Now()
		t.Terms = text.Normalize(t.Text)
		t.EndTime1 = time.Now()
		out <- t
	}

	close(out)
}

func Deduplicate(in <-chan task.Task, out chan<- task.Task) {
	for t := range in {
		t.StartTime2 = time.Now()
		t.Stat = text.Deduplicate(t.Terms)
		t.EndTime2 = time.Now()
		out <- t
	}

	close(out)
}

func CountTF(in <-chan task.Task, out chan<- task.Task) {
	for t := range in {
		t.StartTime3 = time.Now()
		t.TFs = text.CountTF(t.Stat)
		t.EndTime3 = time.Now()
		out <- t
	}

	close(out)
}
