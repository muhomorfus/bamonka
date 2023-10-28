package linear

import (
	"lab_05/internal/pipeline/internal/task"
	"lab_05/internal/pipeline/internal/workers"
	"time"
)

func Pipeline(texts [][]string, log bool) []map[string]float64 {
	tfs := make([]map[string]float64, len(texts))
	resTasks := make([]task.Task, 0, len(texts))

	now := time.Now()

	for _, v := range texts {
		chanNormalize := make(chan task.Task, 1)
		chanDeduplicate := make(chan task.Task, 1)
		chanCountTF := make(chan task.Task, 1)
		chanResult := make(chan task.Task, 1)

		t := task.Task{
			Text: v,
			TimeStat: task.TimeStat{
				StartTime0: now,
			},
		}

		chanNormalize <- t
		close(chanNormalize)

		workers.Normalize(chanNormalize, chanDeduplicate)
		workers.Deduplicate(chanDeduplicate, chanCountTF)
		workers.CountTF(chanCountTF, chanResult)

		resTasks = append(resTasks, <-chanResult)
	}

	for i, v := range resTasks {
		tfs[i] = v.TFs
	}

	if log {
		task.Log(resTasks)
	}

	return tfs
}
