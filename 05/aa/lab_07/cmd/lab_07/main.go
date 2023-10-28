package main

import (
	"flag"
	"fmt"
	"lab_07/internal/graph"
	"lab_07/internal/path/ant"
	"lab_07/internal/path/bruteforce"
	"os"
)

func main() {
	filename := flag.String("filename", "", "имя файла с графом")
	alpha := flag.Float64("alpha", 0.5, "коэффициент стадности")
	rho := flag.Float64("rho", 0.5, "коэффициент испарения")
	t := flag.Int("t", 5, "количество циклов муравьиного алгоритма")
	csvBest := flag.String("filename_csv_best", "", "имя файла для записи лучшего результата в сsv")
	csv := flag.String("filename_csv", "", "имя файла для записи результата в сsv")
	out := flag.Bool("out", true, "выводить ли результат")

	flag.Usage = func() {
		fmt.Println("Неориентированный граф, с элитными муравьями, карта перемещения по портам через океаны. Незамкнутый маршрут. Использование:")
		flag.PrintDefaults()
	}
	flag.Parse()

	g, err := graph.FromFile(*filename)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	mb := bruteforce.New()
	ma := ant.New(*t, *alpha, 1-*alpha, *rho)

	path, lb, ok := mb.MinPath(g)
	if !ok {
		fmt.Println("Ошибка вычисления пути методом полного перебора")
	} else if *out {
		fmt.Println("Полный перебор")
		fmt.Println("Путь:", path)
		fmt.Println("Длина:", lb)
	}

	path, la, ok := ma.MinPath(g)
	if !ok {
		fmt.Println("Ошибка вычисления пути методом муравьиного алгоритма")
	} else if *out {
		fmt.Println()
		fmt.Println("Муравьиный алгоритм")
		fmt.Println("Путь:", path)
		fmt.Println("Длина:", la)
	}

	diff := la - lb
	if diff < 0 {
		diff = -diff
	}

	f, err := os.OpenFile(*csv, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0666)
	if err != nil {
		return
	}

	fmt.Fprintf(f, "%f, %f, %d, %d, %d\n", *alpha, *rho, *t, lb, diff)
	f.Close()

	if diff == 0 {
		f, err := os.OpenFile(*csvBest, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0666)
		if err != nil {
			return
		}

		fmt.Fprintf(f, "%f, %f, %d, %d, %d\n", *alpha, *rho, *t, lb, diff)
		f.Close()
	}
}
