package main

import (
	"os"
	"strconv"
	"sync"
)

func main() {
	n := 5
	m := 10

	
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

	
	server := NewServer(n)
	for i := 0; i < n; i++ {
		server.clients[i] = NewClient(i, n, m, server)
	}

	
	var wg sync.WaitGroup

	
	wg.Add(1)
	go server.Run(&wg)

	
	for _, client := range server.clients {
		wg.Add(1)
		go client.Run(&wg)
	}

	
	wg.Wait()
}