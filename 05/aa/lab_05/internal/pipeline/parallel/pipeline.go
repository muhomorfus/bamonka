package parallel

import (
	"lab_05/internal/pipeline/internal/task"
	"lab_05/internal/pipeline/internal/workers"
	"time"
)

func Pipeline(texts [][]string, log bool) []map[string]float64 {
	tfs := make([]map[string]float64, len(texts))
	resTasks := make([]task.Task, 0, len(texts))

	now := time.Now()

	chanNormalize := make(chan task.Task, len(texts))
	chanDeduplicate := make(chan task.Task, len(texts))
	chanCountTF := make(chan task.Task, len(texts))
	chanResult := make(chan task.Task, len(texts))

	go workers.Normalize(chanNormalize, chanDeduplicate)
	go workers.Deduplicate(chanDeduplicate, chanCountTF)
	go workers.CountTF(chanCountTF, chanResult)

	for _, v := range texts {
		t := task.Task{
			Text: v,
			TimeStat: task.TimeStat{
				StartTime0: now,
			},
		}

		chanNormalize <- t
	}
	close(chanNormalize)

	for t := range chanResult {
		resTasks = append(resTasks, t)
	}

	for i, v := range resTasks {
		tfs[i] = v.TFs
	}

	if log {
		task.Log(resTasks)
	}

	return tfs
}
