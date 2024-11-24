package app

import (
	"os"

	"lab/internal/functional_points"
	"lab/pkg/sdk"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/widget"
	"github.com/charmbracelet/log"
)

type App struct {
	widgets widgets

	model *functional_points.Model
}

type widgets struct {
	fp  sdk.ResultWidget
	fpCorrected       sdk.ResultWidget
	sloc       sdk.ResultWidget
}

func New() *App {
	os.Setenv("FYNE_SCALE","0.75")
	var a App

	a.model = functional_points.New()

	a.createWidgets()

	return &a
}

func (a *App) createWidgets() {
	a.widgets.fp = sdk.Result()
	a.widgets.fpCorrected = sdk.Result()
	a.widgets.sloc = sdk.Result()

	sdk.SetFillWindowFunc(a.setContent)
}

func (a *App) setContent(window fyne.Window) {
	window.SetContent(widget.NewLabel("Loading..."))

	results := []fyne.CanvasObject{
		sdk.Describe("Фунциональные точки", a.widgets.fp),
		sdk.Describe("Скорректированные функциональные точки", a.widgets.fpCorrected),
		sdk.Describe("SLOC", a.widgets.sloc),
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

	a.widgets.fp.SetText(result.Points)
	a.widgets.fpCorrected.SetText(result.PointsCorrected)
	a.widgets.sloc.SetText(result.SLOC)

	sdk.Reload()
}

func (a *App) Run() {
	sdk.Run()
}
