package app

import (
	"lab/internal/cocomo"
	"lab/pkg/sdk"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
	"github.com/charmbracelet/log"
)

type App struct {
	widgets widgets

	model *cocomo.Model
}

type widgets struct {
	laborCost  sdk.ResultWidget
	time       sdk.ResultWidget
	cost       sdk.ResultWidget
	byStage    *customTable
	byCategory *customTable
}

func New() *App {
	var a App

	a.model = cocomo.New()

	a.createWidgets()

	return &a
}

func (a *App) createWidgets() {
	a.widgets.laborCost = sdk.Result()
	a.widgets.time = sdk.Result()
	a.widgets.cost = sdk.Result()
	a.widgets.byStage = a.makeTable([][]string{{}})
	a.widgets.byCategory = a.makeTable([][]string{{}})

	sdk.SetFillWindowFunc(a.setContent)
}

func (a *App) setContent(window fyne.Window) {
	window.SetContent(widget.NewLabel("Loading..."))

	results := []fyne.CanvasObject{
		sdk.Describe("Трудозатраты", a.widgets.laborCost),
		sdk.Describe("Время", a.widgets.time),
		sdk.Describe("Стоимость", a.widgets.cost),
		container.NewAppTabs(
			container.NewTabItem("Разбивка по этапу", a.widgets.byStage),
			container.NewTabItem("Разбивка по виду", a.widgets.byCategory),
		),
	}

	content := sdk.Layout(
		a.model.Widgets(),
		results,
		a.onComputeButtonPressed,
	)

	window.SetContent(content)
}

func (a *App) onComputeButtonPressed() {
	log.Debug("compute button pressed")

	result, err := a.model.Compute()
	if err != nil {
		sdk.ShowError(err)
		return
	}

	a.widgets.laborCost.SetText(result.LaborCost)
	a.widgets.time.SetText(result.Time)
	a.widgets.cost.SetText(result.Cost)

	a.widgets.byStage = a.makeTable(result.ByStage)
	a.widgets.byCategory = a.makeTable(result.ByCategory)

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

	tbl.SetColumnWidth(0, 250)
	tbl.SetColumnWidth(1, 120)
	tbl.SetColumnWidth(2, 120)
	tbl.SetRowHeight(1, 60)
	tbl.SetRowHeight(2, 60)
	tbl.SetRowHeight(3, 60)
	tbl.SetRowHeight(4, 60)
	tbl.SetRowHeight(5, 60)
	tbl.SetRowHeight(6, 60)
	tbl.SetRowHeight(7, 60)
	tbl.SetRowHeight(8, 60)

	return tbl
}

type customTable struct {
	*widget.Table
}

func (t *customTable) MinSize() fyne.Size {
	return fyne.Size{
		Width:  500,
		Height: 400,
	}
}

func (a *App) Run() {
	sdk.Run()
}
