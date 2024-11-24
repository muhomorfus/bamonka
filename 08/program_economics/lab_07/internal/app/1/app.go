package app

import (
	"os"

	"lab/internal/composition_model"
	"lab/pkg/sdk"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/widget"
	"github.com/charmbracelet/log"
)

type App struct {
	widgets widgets

	model *composition_model.Model
}

type widgets struct {
	laborCost sdk.ResultWidget
	time      sdk.ResultWidget
	cost      sdk.ResultWidget
}

func New() *App {
	os.Setenv("FYNE_SCALE", "0.75")

	var a App

	a.model = composition_model.New()

	a.createWidgets()

	return &a
}

func (a *App) createWidgets() {
	a.widgets.laborCost = sdk.Result()
	a.widgets.time = sdk.Result()
	a.widgets.cost = sdk.Result()

	sdk.SetFillWindowFunc(a.setContent)
}

func (a *App) setContent(window fyne.Window) {
	window.SetContent(widget.NewLabel("Loading..."))

	results := []fyne.CanvasObject{
		sdk.Describe("Трудозатраты", a.widgets.laborCost),
		sdk.Describe("Время", a.widgets.time),
		sdk.Describe("Стоимость", a.widgets.cost),
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

	sdk.Reload()
}

func (a *App) Run() {
	sdk.Run()
}
