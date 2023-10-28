package main

import (
	"encoding/json"
	"fmt"
	"github.com/jaswdr/faker"
	"log"
	"math/rand"
	"os"
	"os/signal"
	"syscall"
	"time"
)

var currentID = 0

const tableName = "zakladki"

type Zakladka struct {
	Address string `json:"address"`
	ZoneID  int    `json:"zone_id"`
}

func randomZakladka() Zakladka {
	return Zakladka{
		Address: faker.New().Address().Address(),
		ZoneID:  rand.Intn(100) + 1,
	}
}

func randomZakladkaList(n int) []Zakladka {
	list := make([]Zakladka, 0, n)

	for i := 0; i < n; i++ {
		list = append(list, randomZakladka())
	}

	return list
}

type worker struct {
	n        int
	filename string
}

func newWorker(n int, filename string) *worker {
	return &worker{
		filename: filename,
		n:        n,
	}
}

func (w *worker) work() error {
	f, err := os.Create(w.filename)
	if err != nil {
		return err
	}

	defer f.Close()

	if err := json.NewEncoder(f).Encode(randomZakladkaList(w.n)); err != nil {
		return err
	}

	return nil
}

func filename() string {
	name := fmt.Sprintf("../deployments/nifi/in_file/%06d_%s_%s.json", currentID, tableName, time.Now().Format(time.RFC3339))
	currentID++

	return name
}

func main() {
	rand.Seed(time.Now().UnixNano())
	ticker := time.NewTicker(10 * time.Second)
	w := newWorker(3, filename())

	c := make(chan os.Signal, 1)
	signal.Notify(c, syscall.SIGINT)

	for {
		select {
		case <-ticker.C:
			log.Println("create data")
			if err := w.work(); err != nil {
				log.Printf("error: %s", err)
			}
		case <-c:
			log.Println("shutdown")
			return
		}
	}
}
