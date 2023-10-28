package main

import (
    "time"
    "math/rand"
    "log"
    "os"
    "flag"
    "fmt"
    "io"
)

const (
    rotorsCount = 3
    alphabetCount = 256
)

func shufflePairs(n int) []int {
	s2 := make([]int, n/2)

	for i := range s2 {
		s2[i] = n/2 + i
	}

	shuffled := make([]int, n)
	k := 0
	for i := len(s2); i > 0; i-- {
		index := rand.Intn(i)

		shuffled[k] = s2[index]

		s2[index] = s2[i-1]

		k++
	}

	for i := 0; i < n/2; i++ {
		shuffled[shuffled[i]] = i
	}

	return shuffled
}

func fprintArray(w io.Writer, arr []int) {
    for _, v := range arr {
        fmt.Fprintln(w, v)
    }
}

func main() {
    flag.Parse()

    rotors := make([][]int, rotorsCount)

    rand.Seed(time.Now().Unix())
    for i := range rotors {
        rotors[i] = rand.Perm(alphabetCount)
    }
    reflector := shufflePairs(alphabetCount)
    panel := shufflePairs(alphabetCount)

    file, err := os.Create("../configs/alphabet_count.txt")
    if err != nil {
        log.Fatal("who hui (open)")
    }
    fmt.Fprint(file, alphabetCount)
    file.Close()

    file, err = os.Create("../configs/rotors_count.txt")
    if err != nil {
        log.Fatal("who hui (open)")
    }
    fmt.Fprint(file, rotorsCount)
    file.Close()

    for i := range rotors {
        file, err = os.Create(fmt.Sprintf("../configs/rotor_%d.txt", i+1))
        if err != nil {
            log.Fatal("who hui (open)")
        }
        fprintArray(file, rotors[i])
        file.Close()
    }

    file, err = os.Create("../configs/panel.txt")
    if err != nil {
        log.Fatal("who hui (open)")
    }
    fprintArray(file, panel)
    file.Close()

    file, err = os.Create("../configs/reflector.txt")
    if err != nil {
        log.Fatal("who hui (open)")
    }
    fprintArray(file, reflector)
    file.Close()

    log.Println("success (babka)")
}