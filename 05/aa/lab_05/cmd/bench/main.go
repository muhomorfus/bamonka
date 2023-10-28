package main

import (
	"fmt"
	"lab_05/internal/pipeline/linear"
	"lab_05/internal/pipeline/parallel"
	"lab_05/internal/text"
	"lab_05/internal/utils"
	"log"
	"os"
	"strings"
	"testing"
)

func main() {
	tests := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20}

	timeFiles := make([]*os.File, 2)

	for i := 0; i < 2; i++ {
		timeFile, err := os.Create(fmt.Sprintf("data/time_%02d.txt", i))
		if err != nil {
			log.Fatalln(err)
		}
		timeFiles[i] = timeFile
	}

	row := []string{"Кол-во заявок", "Последовательный", "Параллельный"}
	fmt.Printf("%s \\\\ \\hline \n", strings.Join(row, " & "))

	for _, n := range tests {
		texts, err := text.ReadTexts(fmt.Sprintf("data/tests/%d.txt", n))
		if err != nil {
			utils.ExitErr("ошибка открытия файла: " + err.Error())
		}

		row := []string{fmt.Sprint(n)}

		res0 := testing.Benchmark(func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				linear.Pipeline(texts, false)
			}
		})

		fmt.Fprintf(timeFiles[0], "%d %d\n", n, res0.NsPerOp()/1000)
		row = append(row, fmt.Sprint(res0.NsPerOp()/1000))

		res1 := testing.Benchmark(func(b *testing.B) {
			for i := 0; i < b.N; i++ {
				parallel.Pipeline(texts, false)
			}
		})
		fmt.Fprintf(timeFiles[1], "%d %d\n", n, res1.NsPerOp()/1000)
		row = append(row, fmt.Sprint(res1.NsPerOp()/1000))

		fmt.Printf("%s \\\\ \\hline \n", strings.Join(row, " & "))
	}
}
