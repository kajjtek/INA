package main

import (
	"fmt"
	"sync"
)

type Server struct {
	Queue          chan Message // Niebuforowany kanał = SynchronousQueue
	clients        []*Client
	sendingClients int
}

func NewServer(n int) *Server {
	return &Server{
		Queue:          make(chan Message),
		clients:        make([]*Client, n),
		sendingClients: n,
	}
}

// Metoda uruchamiana jako goroutine
func (s *Server) Run(wg *sync.WaitGroup) {
	defer wg.Done() // Zgłoś zakończenie pracy na koniec działania

	// Główna pętla serwera
	for s.sendingClients > 0 {
		msg := <-s.Queue // Zablokuj i czekaj na wiadomość od klienta

		fmt.Printf("[SERVER] RECEIVED MESSAGE: %d FROM: %d TO: %d\n",
			msg.messageID, msg.sourceID, msg.receiverID)

		if msg.messageID == -1 {
			s.sendingClients--
		} else {
			targetClient := s.clients[msg.receiverID]
			targetClient.Inbox <- msg // Przekazanie do skrzynki odbiorczej
		}
	}

	// Rozesłanie "trucizny" (-999) wyłączającej klientów
	for i, client := range s.clients {
		client.Inbox <- Message{sourceID: -1, messageID: -999, receiverID: i}
	}

	fmt.Println("[SERVER] shutting down")
}