package app

import (
	"errors"
	"fmt"
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
	"strconv"
	"sync"
)

var (
	ErrInvalidFloatInRussian = errors.New("Неверное число")
)

func validateMatrixValue(s string) error {
	_, err := strconv.ParseFloat(s, 64)
	if err != nil {
		return ErrInvalidFloatInRussian
	}

	return nil
}

func (a *App) onValidationMatrixValueChanged(err error) {
	a.mu.Lock()
	defer a.mu.Unlock()

	if err != nil {
		a.invalidEntries++
	} else {
		a.invalidEntries--
	}

	if a.invalidEntries == 0 {
		a.computeButton.Enable()
		a.plotButton.Enable()
	} else {
		a.computeButton.Disable()
		a.plotButton.Disable()
	}
}

type App struct {
	app    fyne.App
	window fyne.Window

	matrix *fyne.Container

	incrementButton *widget.Button
	decrementButton *widget.Button
	n               int
	nLabel          *widget.Label

	computeButton *widget.Button
	plotButton    *widget.Button

	mu             sync.Mutex
	invalidEntries int
}

func (a *App) resizeMatrix(n int) {
	if n < 1 {
		return
	}
	a.n = n
	a.nLabel.SetText(fmt.Sprint(a.n))

	a.matrix.RemoveAll()
	a.matrix = container.NewGridWithColumns(a.n)

	for i := 0; i < a.n*a.n; i++ {
		e := widget.NewEntry()
		e.SetText("0.0")
		e.Validator = validateMatrixValue
		e.SetOnValidationChanged(a.onValidationMatrixValueChanged)

		a.matrix.Add(e)
	}
}

func New() *App {
	var a App

	a.app = app.New()
	a.window = a.app.NewWindow("lab_02")
	a.window.SetFixedSize(true)
	a.window.Resize(fyne.NewSize(1100, 700))

	a.matrix = container.NewGridWithColumns(0)
	a.nLabel = widget.NewLabel("0")
	a.n = 0

	a.nLabel.Alignment = fyne.TextAlignCenter
	a.incrementButton = widget.NewButton("+", func() {
		a.resizeMatrix(a.n + 1)
		a.show()
	})
	a.decrementButton = widget.NewButton("-", func() {
		a.resizeMatrix(a.n - 1)
		a.show()
	})
	a.resizeMatrix(3)

	a.computeButton = widget.NewButton("Рассчитать", func() {
		a.showResultWindow()
	})
	a.plotButton = widget.NewButton("Показать график", func() {

	})

	return &a
}

func (a *App) setContent() {
	leftMenu := container.NewVBox(
		widget.NewLabel("Размерность матрицы"),
		container.NewGridWithColumns(3, a.decrementButton, a.nLabel, a.incrementButton),
		widget.NewSeparator(),
		a.computeButton,
		a.plotButton,
	)

	body := container.NewGridWithColumns(1, a.matrix)
	a.window.SetContent(container.NewBorder(nil, nil, leftMenu, nil, body))
}

type minSizedCard struct {
	*widget.Card
	width, height float32
}

func (m minSizedCard) MinSize() fyne.Size {
	return fyne.NewSize(m.width, m.height)
}

func squareLabel(text string) fyne.CanvasObject {
	return minSizedCard{
		Card:   widget.NewCard("", "", widget.NewLabel(text)),
		width:  120,
		height: 60,
	}
}

func (a *App) showResultWindow() {
	window := a.app.NewWindow("Результат")
	window.SetFixedSize(false)

	grid := container.NewGridWithColumns(a.n + 1)

	grid.Add(squareLabel("P"))
	for i := 0; i < a.n; i++ {
		grid.Add(squareLabel("1.0"))
	}

	grid.Add(squareLabel("t"))
	for i := 0; i < a.n; i++ {
		grid.Add(squareLabel("-2.21"))
	}

	window.SetContent(grid)
	window.Show()
}

func (a *App) Run() {
	a.setContent()
	a.window.ShowAndRun()
}

func (a *App) show() {
	a.setContent()
	a.window.Show()
}
