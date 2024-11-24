package cocomo

type Result struct {
	// LaborCost трудозатраты.
	LaborCost string
	Time      string
	Cost      string

	ByStage    [][]string
	ByCategory [][]string
}
