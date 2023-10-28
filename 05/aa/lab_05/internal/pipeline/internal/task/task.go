package task

import (
	"fmt"
	"math"
	"time"
)

type Task struct {
	Text  []string
	Terms []string
	Stat  map[string]int
	TFs   map[string]float64

	TimeStat
}

type TimeStat struct {
	StartTime0 time.Time

	StartTime1 time.Time
	EndTime1   time.Time

	StartTime2 time.Time
	EndTime2   time.Time

	StartTime3 time.Time
	EndTime3   time.Time
}

func min(a, b int64) int64 {
	if a < b {
		return a
	}

	return b
}

func max(a, b int64) int64 {
	if a > b {
		return a
	}

	return b
}

func Log(tasks []Task) {
	start := tasks[0].StartTime0

	fmt.Printf("%10s & %10s & %10s & %10s \\\\ \\hline\n", "№ заявки", "Этап", "Начало", "Конец")
	for i, v := range tasks {
		fmt.Printf("%10d & %10d & %10d & %10d \\\\ \\hline\n", i+1, 1, v.StartTime1.Sub(start).Microseconds(), v.EndTime1.Sub(start).Microseconds())
		fmt.Printf("%10d & %10d & %10d & %10d \\\\ \\hline\n", i+1, 2, v.StartTime2.Sub(start).Microseconds(), v.EndTime2.Sub(start).Microseconds())
		fmt.Printf("%10d & %10d & %10d & %10d \\\\ \\hline\n", i+1, 3, v.StartTime3.Sub(start).Microseconds(), v.EndTime3.Sub(start).Microseconds())
	}

	fmt.Println()

	stayGen1 := int64(0)
	stayGen2 := int64(0)
	stayGen3 := int64(0)

	stayMin1 := int64(math.MaxInt64)
	stayMin2 := int64(math.MaxInt64)
	stayMin3 := int64(math.MaxInt64)

	stayMax1 := int64(0)
	stayMax2 := int64(0)
	stayMax3 := int64(0)

	timeGen := int64(0)
	timeMin := int64(math.MaxInt64)
	timeMax := int64(0)

	for _, v := range tasks {
		stay1 := v.StartTime1.Sub(v.StartTime0).Microseconds()
		stay2 := v.StartTime2.Sub(v.EndTime1).Microseconds()
		stay3 := v.StartTime3.Sub(v.EndTime2).Microseconds()
		curTime := v.EndTime3.Sub(v.StartTime0).Microseconds()

		stayGen1 += stay1
		stayGen2 += stay2
		stayGen3 += stay3

		stayMin1 = min(stayMin1, stay1)
		stayMin2 = min(stayMin2, stay2)
		stayMin3 = min(stayMin3, stay3)

		stayMax1 = max(stayMax1, stay1)
		stayMax2 = max(stayMax2, stay2)
		stayMax3 = max(stayMax3, stay3)

		timeGen += curTime
		timeMin = min(timeMin, curTime)
		timeMax = max(timeMax, curTime)
	}

	fmt.Printf("Stay gen:\n %5d & %5d & %5d\n\n", stayGen1, stayGen2, stayGen3)
	fmt.Printf("Stay min:\n %5d & %5d & %5d\n\n", stayMin1, stayMin2, stayMin3)
	fmt.Printf("Stay max:\n %5d & %5d & %5d\n\n", stayMax1, stayMax2, stayMax3)
	fmt.Printf("Stay avg:\n %5d & %5d & %5d\n\n", stayGen1/int64(len(tasks)), stayGen2/int64(len(tasks)), stayGen3/int64(len(tasks)))

	fmt.Printf("In system min:\n%5d\n\n", timeMin)
	fmt.Printf("In system max:\n%5d\n\n", timeMax)
	fmt.Printf("In system avg:\n%5d\n\n", timeGen/int64(len(tasks)))
}
