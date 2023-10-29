package main

import (
	"flag"
	"log"
)

func main() {
	filename := flag.String("file", "", "name of file with data")
	mode := flag.String("mode", "generate", "mode of work program")
	flag.Parse()

	err := Run(*filename, *mode)
	if err != nil {
		log.Fatalln(err)
	}
}
