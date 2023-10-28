package main

import (
	"flag"
	"fmt"
	dl "lab_01/internal/algs/damerau_levenshtein"
	dlr "lab_01/internal/algs/damerau_levenshtein_recursive"
	dlrc "lab_01/internal/algs/damerau_levenshtein_recursive_cached"
	l "lab_01/internal/algs/levenshtein"
)

func main() {
	s1 := flag.String("s1", "", "first string for compare")
	s2 := flag.String("s2", "", "second string for compare")
	flag.Parse()

	fmt.Println(l.New().Diff(*s1, *s2))
	fmt.Println(dl.New().Diff(*s1, *s2))
	fmt.Println(dlr.New().Diff(*s1, *s2))
	fmt.Println(dlrc.New().Diff(*s1, *s2))
}
