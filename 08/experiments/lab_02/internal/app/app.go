package app

import (
	"lab/internal/experiment"
	"lab/pkg/sdk"
	"lab/pkg/sdk/advanced_widgets"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/widget"
	"github.com/charmbracelet/log"
)

type App struct {
	widgets widgets

	experiment *experiment.Experiment
}

type widgets struct {
	limitProcessed *advanced_widgets.NumericalEntry

	generatorIntencity *advanced_widgets.NumericalEntry
	generatorDelta     *advanced_widgets.NumericalEntry

	processorIntencity *advanced_widgets.NumericalEntry

	normalizedLinearEquation    *widget.Label
	linearEquation              *widget.Label
	normalizedNonLinearEquation *widget.Label
	nonLinearEquation           *widget.Label
	table                       *customTable
}

func New() *App {
	var a App

	a.experiment = experiment.New()
	a.createWidgets()

	return &a
}

func (a *App) createWidgets() {
	a.widgets.limitProcessed = advanced_widgets.NewIntEntry(1000)
	a.widgets.generatorIntencity = advanced_widgets.NewFloatEntry(2)
	a.widgets.generatorDelta = advanced_widgets.NewFloatEntry(0.15)
	a.widgets.processorIntencity = advanced_widgets.NewFloatEntry(6.5)
	a.widgets.normalizedLinearEquation = widget.NewLabel("")
	a.widgets.linearEquation = widget.NewLabel("")
	a.widgets.normalizedNonLinearEquation = widget.NewLabel("")
	a.widgets.nonLinearEquation = widget.NewLabel("")
	a.widgets.table = a.makeTable([][]string{{}})

	sdk.SetFillWindowFunc(a.setContent)
}

func (a *App) setContent(window fyne.Window) {
	window.SetContent(widget.NewLabel("Loading..."))

	params := []fyne.CanvasObject{
		sdk.Describe("Количество заявок", a.widgets.limitProcessed),
		sdk.Describe("Интенсивность генерации", a.widgets.generatorIntencity),
		sdk.Describe("Отклонение генерации", a.widgets.generatorDelta),
		sdk.Describe("Интенсивность обслуживания", a.widgets.processorIntencity),
	}

	results := []fyne.CanvasObject{
		a.widgets.table,
		sdk.Describe("Линейное нормализованное", a.widgets.normalizedLinearEquation),
		sdk.Describe("Линейное", a.widgets.linearEquation),
		sdk.Describe("Частично линейное нормализованное", a.widgets.normalizedNonLinearEquation),
		sdk.Describe("Частично линейное", a.widgets.nonLinearEquation),
	}

	content := sdk.Layout(
		params,
		results,
		a.onComputeButtonPressed,
	)

	window.SetContent(content)
}

func (a *App) onComputeButtonPressed() {
	log.Debug("compute button pressed")

	limitProcessed, err := a.widgets.limitProcessed.GetInt()
	if err != nil {
		sdk.ShowError(err)
		return
	}

	err = a.experiment.MakeExperiment(limitProcessed)
	if err != nil {
		sdk.ShowError(err)
		return
	}

	table := a.experiment.CreateTable()

	generatorIntencity, err := a.widgets.generatorIntencity.GetFloat()
	if err != nil {
		sdk.ShowError(err)
		return
	}

	generatorDelta, err := a.widgets.generatorDelta.GetFloat()
	if err != nil {
		sdk.ShowError(err)
		return
	}

	processorIntencity, err := a.widgets.processorIntencity.GetFloat()
	if err != nil {
		sdk.ShowError(err)
		return
	}

	res, err := a.experiment.Check(limitProcessed, generatorIntencity, generatorDelta, processorIntencity)
	if err != nil {
		sdk.ShowError(err)
		return
	}

	table = append(table, res)

	a.widgets.table = a.makeTable(table)
	a.widgets.normalizedLinearEquation.SetText(a.experiment.MakeLinearNormalizedEquation())
	a.widgets.linearEquation.SetText(a.experiment.MakeLinearEquation())
	a.widgets.normalizedNonLinearEquation.SetText(a.experiment.MakeNonLinearNormalizedEquation())
	a.widgets.nonLinearEquation.SetText(a.experiment.MakeNonLinearEquation())

	sdk.Reload()
}

func (a *App) makeTable(data [][]string) *customTable {
	tbl := &customTable{
		widget.NewTable(
			func() (int, int) {
				return len(data), len(data[0])
			},
			func() fyne.CanvasObject {
				return widget.NewLabel("wide content")
			},
			func(i widget.TableCellID, o fyne.CanvasObject) {
				o.(*widget.Label).SetText(data[i.Row][i.Col])
			},
		),
	}

	for i := range 16 {
		tbl.SetColumnWidth(i, 60)
	}
	// tbl.SetColumnWidth(1, 120)
	// tbl.SetColumnWidth(2, 120)
	// tbl.SetRowHeight(1, 60)
	// tbl.SetRowHeight(2, 60)
	// tbl.SetRowHeight(3, 60)
	// tbl.SetRowHeight(4, 60)
	// tbl.SetRowHeight(5, 60)
	// tbl.SetRowHeight(6, 60)
	// tbl.SetRowHeight(7, 60)
	// tbl.SetRowHeight(8, 60)

	return tbl
}

type customTable struct {
	*widget.Table
}

func (t *customTable) MinSize() fyne.Size {
	return fyne.Size{
		Width:  1200,
		Height: 400,
	}
}

func (a *App) Run() {
	sdk.Run()
}
