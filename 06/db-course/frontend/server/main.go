package main

import (
	"bytes"
	"flag"
	"fmt"
	"log"
	"net/http"
	"os"
	"strings"
)

const (
	backendVariableName = "{{ BASE_PATH }}"
)

func main() {
	port := flag.Int("port", 0, "port of frontend server")
	backendURL := flag.String("backend", "", "url of backend")

	flag.Parse()

	http.HandleFunc("/assets/", func(w http.ResponseWriter, r *http.Request) {
		filename := "dist" + r.URL.Path
		content, err := os.ReadFile(filename)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			_, _ = fmt.Fprintln(w, err)
			return
		}

		if strings.HasSuffix(filename, ".js") {
			content = bytes.ReplaceAll(content, []byte(backendVariableName), []byte(*backendURL))
			if err := os.WriteFile(filename, content, 0666); err != nil {
				w.WriteHeader(http.StatusInternalServerError)
				_, _ = fmt.Fprintln(w, err)
				return
			}
		}

		http.ServeFile(w, r, filename)
	})

	http.HandleFunc("/favicon.ico", func(w http.ResponseWriter, r *http.Request) {
		http.ServeFile(w, r, "dist/favicon.ico")
	})

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		http.ServeFile(w, r, "dist/index.html")
	})

	log.Printf("Serving on port %d", *port)
	log.Fatal(http.ListenAndServe(fmt.Sprintf(":%d", *port), nil))
}
