package main

import (
	"flag"
	"fmt"
	"lab_05/internal/pipeline/linear"
	"lab_05/internal/pipeline/parallel"
	"lab_05/internal/text"
	"lab_05/internal/utils"
)

func main() {
	filename := flag.String("filename", "", "имя файла с текстами")
	mode := flag.String("mode", "parallel", "тип используемого конвейера")
	log := flag.Bool("log", false, "выводить ли сервисные логи")
	printResult := flag.Bool("result", true, "выводить ли результат")

	flag.Usage = func() {
		fmt.Println("Подсчет частоты терма для слов текста. Использование:")
		flag.PrintDefaults()
	}
	flag.Parse()

	texts, err := text.ReadTexts(*filename)
	if err != nil {
		utils.ExitErr("ошибка открытия файла: " + err.Error())
	}

	var result []map[string]float64
	if *mode == "parallel" {
		result = parallel.Pipeline(texts, *log)
	} else if *mode == "linear" {
		result = linear.Pipeline(texts, *log)
	} else {
		utils.ExitErr("неверный тип запуска конвейера")
	}

	if *printResult {
		for i, v := range result {
			fmt.Printf("TF для текста %d:\n", i)

			for k, v1 := range v {
				fmt.Printf("\t%s: %f\n", k, v1)
			}
		}
	}
}
