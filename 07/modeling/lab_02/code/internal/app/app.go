package app

import (
	"errors"
	"fmt"
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/theme"
	"fyne.io/fyne/v2/widget"
	"lab_02/internal/model"
	"log"
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

	log.Println("changed validation state", err)

	if err != nil {
		a.invalidEntries++
	} else {
		a.invalidEntries--
	}

	if a.invalidEntries == 0 {
		a.computeButton.Enable()
	} else {
		a.computeButton.Disable()
	}
}

type App struct {
	app    fyne.App
	window fyne.Window

	matrix  *fyne.Container
	entries []*widget.Entry

	p        *fyne.Container
	pEntries []*widget.Entry

	t        *fyne.Container
	tEntries []*widget.Entry

	incrementButton *widget.Button
	decrementButton *widget.Button
	n               int
	nLabel          *widget.Label

	errorLabel *widget.Label

	computeButton *widget.Button

	mu             sync.Mutex
	invalidEntries int
}

func (a *App) resizeMatrix(n int) {
	if n < 1 {
		return
	}
	a.n = n
	a.entries = make([]*widget.Entry, 0, a.n*a.n)
	a.nLabel.SetText(fmt.Sprint(a.n))

	a.matrix.RemoveAll()
	a.matrix = container.NewGridWithColumns(a.n)

	a.p.RemoveAll()
	a.p = container.NewGridWithColumns(a.n)
	a.pEntries = make([]*widget.Entry, 0, a.n)

	a.t.RemoveAll()
	a.t = container.NewGridWithColumns(a.n)
	a.tEntries = make([]*widget.Entry, 0, a.n)

	for i := 0; i < a.n*a.n; i++ {
		e := widget.NewEntry()
		e.SetText("1.0")
		e.Validator = validateMatrixValue
		e.SetOnValidationChanged(a.onValidationMatrixValueChanged)

		a.matrix.Add(e)
		a.entries = append(a.entries, e)
	}

	for i := 0; i < a.n; i++ {
		e := widget.NewEntry()
		e.SetText("")
		e.Disable()
		a.p.Add(e)
		a.pEntries = append(a.pEntries, e)
	}

	for i := 0; i < a.n; i++ {
		e := widget.NewEntry()
		e.SetText("")
		e.Disable()
		a.t.Add(e)
		a.tEntries = append(a.tEntries, e)
	}
}

func New() *App {
	var a App

	a.app = app.New()
	a.app.Settings().SetTheme(theme.LightTheme())
	a.window = a.app.NewWindow("lab_02")
	a.window.Resize(fyne.NewSize(600, 600))

	a.matrix = container.NewGridWithColumns(0)
	a.p = container.NewGridWithColumns(0)
	a.t = container.NewGridWithColumns(0)
	a.nLabel = widget.NewLabel("0")
	a.n = 0

	a.nLabel.Alignment = fyne.TextAlignCenter
	a.incrementButton = widget.NewButton("+", func() {
		log.Println("incrementing number of states")
		a.resizeMatrix(a.n + 1)
		a.show()
	})
	a.decrementButton = widget.NewButton("-", func() {
		log.Println("decrementing number of states")
		a.resizeMatrix(a.n - 1)
		a.show()
	})
	a.resizeMatrix(3)

	a.computeButton = widget.NewButton("Рассчитать", func() {
		log.Println("start computing")
		a.compute()
	})

	a.errorLabel = widget.NewLabel("")
	a.errorLabel.Hidden = true
	a.errorLabel.TextStyle.Bold = true

	return &a
}

func (a *App) setContent() {
	bottom := container.NewVBox(
		container.NewGridWithColumns(3, a.decrementButton, a.nLabel, a.incrementButton),
		a.computeButton,
		a.errorLabel,
		widget.NewLabel("Вероятность"),
		a.p,
		widget.NewLabel("Время"),
		a.t,
	)

	body := container.NewGridWithColumns(
		1,
		a.matrix,
	)
	a.window.SetContent(container.NewBorder(nil, bottom, nil, nil, body))
}

func (a *App) compute() {
	a.errorLabel.Hide()

	elements := make([]float64, a.n*a.n)
	for i, e := range a.entries {
		elements[i], _ = strconv.ParseFloat(e.Text, 64)
	}

	m := model.New(elements, a.n)
	probabilities, err := m.Probabilities()
	if err != nil {
		a.errorLabel.SetText("Не удается решить систему.")
		a.errorLabel.Show()
		log.Println(err)
		return
	}

	times, err := m.Times()
	if err != nil {
		a.errorLabel.SetText("Не удается решить систему.")
		a.errorLabel.Show()
		log.Println(err)
		return
	}

	for i, p := range probabilities {
		a.pEntries[i].SetText(fmt.Sprintf("%.4f", p))
	}

	for i, t := range times {
		a.tEntries[i].SetText(fmt.Sprintf("%.4f", t))
	}
}

func (a *App) Run() {
	a.setContent()
	log.Println("running application")
	a.window.ShowAndRun()
}

func (a *App) show() {
	a.setContent()
	a.window.Show()
}
