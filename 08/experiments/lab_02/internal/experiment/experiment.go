package experiment

import (
	"lab/internal/factor"
	"lab/internal/model"
	"lab/internal/planner"
)

type Experiment struct {
	planner *planner.Planner
}

func New() *Experiment {
	return &Experiment{
		planner: planner.New(
			factor.New(0.5, 2),   // Generator intencity.
			factor.New(0.1, 0.3), // Generator delta.
			factor.New(5, 7),     // Processor intencity.
		),
	}
}

func (e *Experiment) MakeExperiment(limit int) error {
	for i := range e.planner.ExperimentsNumber {
		plan := e.planner.GetPlan(i)
		baseModel := model.NewBaseModel(plan[0], plan[1], plan[2])
		m := model.NewEventModel(baseModel)

		result, err := m.Compute(limit)
		if err != nil {
			return err
		}

		e.planner.SaveResult(i, result)
	}

	e.planner.ComputeCoefficients()

	return nil
}

func (e *Experiment) CreateTable() [][]string {
	return e.planner.CreateTable()
}

func (e *Experiment) MakeLinearEquation() string {
	return e.planner.MakeLinearEquation()
}

func (e *Experiment) MakeLinearNormalizedEquation() string {
	return e.planner.MakeLinearNormalizedEquation()
}

func (e *Experiment) MakeNonLinearNormalizedEquation() string {
	return e.planner.MakeNonLinearNormalizedEquation()
}

func (e *Experiment) MakeNonLinearEquation() string {
	return e.planner.MakeNonLinearEquation()
}

func (e *Experiment) Check(limit int, generatorIntencity, generatorDelta, processorIntencity float64) ([]string, error) {
	baseModel := model.NewBaseModel(generatorIntencity, generatorDelta, processorIntencity)
	m := model.NewEventModel(baseModel)

	result, err := m.Compute(limit)
	if err != nil {
		return nil, err
	}

	return e.planner.AddResultRow(result, generatorIntencity, generatorDelta, processorIntencity), nil
}
