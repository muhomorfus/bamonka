package faker

import (
	"fmt"
	"math/rand"
)

type machineType int

const (
	pc machineType = iota
	gc
)

var machineBrandsPC = []string{"Samsung", "Apple", "Huawei", "Xiaomi",
	"Asus", "Acer", "hp", "Lenovo", "HyperPC", "HydraComputers", "Nokia"}

var machineBrandsGC = []string{"XBox", "PlayStation"}

var machineModels = make([]string, 0, 100)

func (mt machineType) String() string {
	if mt == pc {
		return "computer"
	} else {
		return "game console"
	}
}

type MachineInfo struct {
	Type  string
	Brand string
	Model string
}

var alphabet = []byte("ABCDEFGHIJKLMNOPQRSTUVWXYZ")

func genMachineModel() string {
	format := "%c%c%04d"
	flag := true
	var model string

	for {
		model = fmt.Sprintf(format, ArrayElement(alphabet), ArrayElement(alphabet),
			IntWithLimits(1000, 10000))

		flag = true
		for _, m := range machineModels {
			if model == m {
				flag = false
				break
			}
		}

		if flag {
			break
		}
	}

	machineModels = append(machineModels, model)

	return model
}

func Machine() MachineInfo {
	t := machineType(rand.Int() % 2)

	var brand string
	if t == pc {
		brand = ArrayElement(machineBrandsPC)
	} else {
		brand = ArrayElement(machineBrandsGC)
	}

	model := genMachineModel()

	return MachineInfo{
		Type:  t.String(),
		Brand: brand,
		Model: model,
	}
}
