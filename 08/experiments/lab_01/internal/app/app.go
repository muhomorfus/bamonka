package app

import (
	"lab_01/internal/model"
	"lab_01/pkg/sdk"
	"lab_01/pkg/sdk/advanced_widgets"
	"strconv"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/widget"
	"github.com/charmbracelet/log"
)

type App struct {
	widgets widgets

	baseModel *model.BaseModel
	model     methodModel
}

type widgets struct {
	queueLen          sdk.ResultWidget
	generateIntencity sdk.ResultWidget
	processIntencity  sdk.ResultWidget
	load              sdk.ResultWidget
	theoryLoad        sdk.ResultWidget
	waitTime          sdk.ResultWidget
	limitProcessed    *advanced_widgets.NumericalEntry
}

func New() *App {
	var a App

	a.baseModel = model.NewBaseModel()
	a.model = model.NewEventModel(a.baseModel)

	a.createWidgets()

	return &a
}

func (a *App) createWidgets() {
	a.widgets.queueLen = sdk.Result()
	a.widgets.generateIntencity = sdk.Result()
	a.widgets.processIntencity = sdk.Result()
	a.widgets.load = sdk.Result()
	a.widgets.theoryLoad = sdk.Result()
	a.widgets.waitTime = sdk.Result()
	a.widgets.limitProcessed = advanced_widgets.NewIntEntry(1000)

	sdk.SetFillWindowFunc(a.setContent)
}

func (a *App) setContent(window fyne.Window) {
	window.SetContent(widget.NewLabel("Loading..."))

	params := append([]fyne.CanvasObject{
		sdk.Describe("Количество заявок", a.widgets.limitProcessed),
	}, a.baseModel.Widgets()...)

	results := []fyne.CanvasObject{
		sdk.Describe("Размер очереди", a.widgets.queueLen),
		sdk.Describe("Интенсивность генерации", a.widgets.generateIntencity),
		sdk.Describe("Интенсивность обработки", a.widgets.processIntencity),
		sdk.Describe("Загрузка", a.widgets.load),
		sdk.Describe("Теоретическая загрузка", a.widgets.theoryLoad),
		sdk.Describe("Среднее время ожидания", a.widgets.waitTime),
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

	result, err := a.model.Compute(limitProcessed, 100)
	if err != nil {
		sdk.ShowError(err)
		return
	}

	a.widgets.queueLen.SetText(strconv.Itoa(result.QueueLen))
	a.widgets.generateIntencity.SetText(strconv.FormatFloat(result.GenerateIntencity, 'f', 2, 64))
	a.widgets.processIntencity.SetText(strconv.FormatFloat(result.ProcessIntencity, 'f', 2, 64))
	a.widgets.load.SetText(strconv.FormatFloat(result.Load, 'f', 2, 64))
	a.widgets.theoryLoad.SetText(strconv.FormatFloat(result.TheoryLoad, 'f', 2, 64))
	a.widgets.waitTime.SetText(strconv.FormatFloat(result.AverageTime, 'f', 3, 64))
}

func (a *App) Run() {
	sdk.Run()
}

type methodModel interface {
	Compute(limit, percent int) (*model.Result, error)
}
