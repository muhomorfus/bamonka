package app

import (
	"errors"
	"fmt"
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/theme"
	"fyne.io/fyne/v2/widget"
	"github.com/charmbracelet/log"
	"image/color"
	"lab_03/internal/rand"
	"lab_03/internal/rand/check"
	"strconv"
	"sync"
)

const (
	numbers  = 10
	variants = 3

	generateCount = 1000
)

var (
	errIntNotInRange = errors.New("value not in range from 0 to 9")
)

func validateInt(s string) error {
	v, err := strconv.Atoi(s)
	if err != nil {
		return err
	}

	if v < 0 || v > 9 {
		return errIntNotInRange
	}

	return nil
}

func (a *App) onValidationIntChanged(err error) {
	a.mu.Lock()
	defer a.mu.Unlock()

	log.Warn("changed validation state", "error", err)

	if err != nil {
		a.invalidEntries++
	} else {
		a.invalidEntries--
	}

	if a.invalidEntries == 0 {
		a.checkButton.Enable()
	} else {
		a.checkButton.Disable()
	}
}

type App struct {
	app    fyne.App
	window fyne.Window

	tabularGenerators       [variants]*rand.Tabular
	computationalGenerators [variants]*rand.Computational

	tabularButtons       [variants][numbers]*widget.Button
	computationalButtons [variants][numbers]*widget.Button

	tabularChecks       [variants]*widget.Button
	computationalChecks [variants]*widget.Button

	generateButton *widget.Button

	checkEntries [10]*widget.Entry
	checkButton  *widget.Button
	check        *widget.Label

	content *widget.Label

	mu             sync.Mutex
	invalidEntries int
}

type myTheme struct {
	fyne.Theme
}

func (t *myTheme) Color(n fyne.ThemeColorName, v fyne.ThemeVariant) color.Color {
	if n == theme.ColorNameBackground {
		return color.NRGBA{R: 0xff, G: 0xdd, B: 0xdd, A: 0xff}
	}

	return t.Theme.Color(n, v)
}

var pink = &myTheme{theme.LightTheme()}

func New() *App {
	log.SetLevel(log.DebugLevel)

	var a App

	a.app = app.New()
	a.app.Settings().SetTheme(pink)
	a.window = a.app.NewWindow("lab_03")
	a.window.Resize(fyne.NewSize(900, 400))

	for i := 0; i < variants; i++ {
		a.tabularGenerators[i] = rand.NewTabular(i + 1)
		a.computationalGenerators[i] = rand.NewComputational(i+1, 228, 2424, 542434, 123456)
	}

	for i := 0; i < numbers; i++ {
		a.checkEntries[i] = widget.NewEntry()
		a.checkEntries[i].Text = "0"
		a.checkEntries[i].Validator = validateInt
		a.checkEntries[i].SetOnValidationChanged(a.onValidationIntChanged)
	}

	a.check = newLabelCenter("")

	a.checkButton = widget.NewButton("Проверить", a.checkButtonPressed)

	a.generateButton = widget.NewButton("Сгенерировать", a.generateButtonPressed)

	a.content = widget.NewLabel("")
	a.content.TextStyle.Monospace = true

	for i := 0; i < variants; i++ {
		for j := 0; j < numbers; j++ {
			a.tabularButtons[i][j] = widget.NewButton("0", func() {})
			a.computationalButtons[i][j] = widget.NewButton("0", func() {})
		}
		a.tabularChecks[i] = widget.NewButton("0", func() {})
		a.computationalChecks[i] = widget.NewButton("0", func() {})
	}

	return &a
}

func (a *App) setContent() {
	log.Info("setting content")

	tabularGrid := container.NewGridWithColumns(3)
	for j := 0; j < numbers; j++ {
		for i := 0; i < variants; i++ {
			tabularGrid.Add(a.tabularButtons[i][j])
		}
	}

	for i := 0; i < variants; i++ {
		tabularGrid.Add(a.tabularChecks[i])
	}

	computationalGrid := container.NewGridWithColumns(3)
	for j := 0; j < numbers; j++ {
		for i := 0; i < variants; i++ {
			computationalGrid.Add(a.computationalButtons[i][j])
		}
	}

	for i := 0; i < variants; i++ {
		computationalGrid.Add(a.computationalChecks[i])
	}

	body := container.NewGridWithColumns(2, widget.NewCard("Табличный способ", "", tabularGrid), widget.NewCard("Алгоритмический способ", "", computationalGrid))

	checkInputGrid := container.NewGridWithColumns(11)
	for i := 0; i < numbers; i++ {
		checkInputGrid.Add(a.checkEntries[i])
	}
	checkInputGrid.Add(a.check)

	bottom := container.NewVBox(a.generateButton, checkInputGrid, a.checkButton)

	a.window.SetContent(container.NewBorder(nil, bottom, nil, nil, body))
}

func (a *App) Run() {
	a.setContent()
	log.Info("running application")
	a.window.ShowAndRun()
	log.Info("exiting")
}

func (a *App) checkButtonPressed() {
	log.Debug("check button pressed")

	sequence := make([]int, numbers)
	for i := range sequence {
		sequence[i], _ = strconv.Atoi(a.checkEntries[i].Text)
	}

	p := check.IsRandom(sequence)
	a.check.SetText(formatFloat(p))

	log.Debug("check button pressed handler finished", "p", p)
}

func (a *App) generateButtonPressed() {
	log.Debug("generate button pressed")

	var tabularResults [variants][]int
	for i := 0; i < variants; i++ {
		for j := 0; j < generateCount; j++ {
			tabularResults[i] = append(tabularResults[i], a.tabularGenerators[i].Rand())
		}
	}

	for i := 0; i < variants; i++ {
		for j := 0; j < numbers; j++ {
			a.tabularButtons[i][j].SetText(fmt.Sprintf("%d", tabularResults[i][j]))
		}
	}
	for i := 0; i < variants; i++ {
		a.tabularChecks[i].SetText(fmt.Sprintf("%.4f", check.IsRandom(tabularResults[i])))
	}

	var computationalResults [variants][]int
	for i := 0; i < variants; i++ {
		for j := 0; j < generateCount; j++ {
			computationalResults[i] = append(computationalResults[i], a.computationalGenerators[i].Rand())
		}
	}

	for i := 0; i < variants; i++ {
		for j := 0; j < numbers; j++ {
			a.computationalButtons[i][j].SetText(fmt.Sprintf("%d", computationalResults[i][j]))
		}
	}
	for i := 0; i < variants; i++ {
		a.computationalChecks[i].SetText(fmt.Sprintf("%.4f", check.IsRandom(computationalResults[i])))
	}

	log.Debug("generate button pressed handler finished")
}

func newLabelRight(text string) *widget.Label {
	result := widget.NewLabel(text)
	result.Alignment = fyne.TextAlignTrailing
	return result
}

func newLabelCenter(text string) *widget.Label {
	result := widget.NewLabel(text)
	result.Alignment = fyne.TextAlignCenter
	return result
}

func newLabelHeader(text string) *widget.Label {
	result := widget.NewLabel(text)
	result.Alignment = fyne.TextAlignCenter
	result.TextStyle.Bold = true
	return result
}

func formatFloat(n float64) string {
	return fmt.Sprintf("%.4f", n)
}

func formatInt(n int) string {
	return fmt.Sprintf("%d", n)
}
