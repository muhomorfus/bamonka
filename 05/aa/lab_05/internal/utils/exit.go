package utils

import (
	"fmt"
	"os"
)

func ExitErr(msg interface{}) {
	fmt.Println(msg)
	os.Exit(1)
}
