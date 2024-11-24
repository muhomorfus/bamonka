package main

import "lab/internal/app"

func main() {
	// e := experiment.New()
	// fmt.Println(e.MakeExperiment(100))
	// // plan := p.CreatePlan()
	// // for j := range plan.Header {
	// // 	fmt.Printf("%5s", plan.Header[j])
	// // }
	// // fmt.Println()
	// // for i := range plan.Matrix {
	// // 	for j := range plan.Matrix[i] {
	// // 		fmt.Printf("%5.0f", plan.Matrix[i][j])
	// // 	}

	// // 	fmt.Println()
	// // }
	a := app.New()
	a.Run()
}
