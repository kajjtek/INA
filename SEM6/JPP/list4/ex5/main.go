package main

import (
	"os"
	"strconv"
	"sync"
)

func main() {
	n := 5
	m := 10

	// Parsowanie argumentów wejściowych (os.Args[0] to nazwa programu)
	if len(os.Args) >= 2 {
		if val, err := strconv.Atoi(os.Args[1]); err == nil {
			n = val
		}
	}
	if len(os.Args) >= 3 {
		if val, err := strconv.Atoi(os.Args[2]); err == nil {
			m = val
		}
	}

	// Inicjalizacja
	server := NewServer(n)
	for i := 0; i < n; i++ {
		server.clients[i] = NewClient(i, n, m, server)
	}

	// WaitGroup śledzi ile goroutines musi się jeszcze zakończyć
	var wg sync.WaitGroup

	// Uruchomienie serwera
	wg.Add(1)
	go server.Run(&wg)

	// Uruchomienie klientów
	for _, client := range server.clients {
		wg.Add(1)
		go client.Run(&wg)
	}

	// Zablokuj główny wątek (main), dopóki wszystkie goroutines nie wywołają wg.Done()
	wg.Wait()
}