package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

type Client struct {
	id       int
	n        int
	msgLeft  int
	server   *Server
	Inbox    chan Message // Buforowany kanał = LinkedBlockingQueue
	received int
}

func NewClient(id, n, messages int, server *Server) *Client {
	return &Client{
		id:       id,
		n:        n,
		msgLeft:  messages,
		server:   server,
		Inbox:    make(chan Message, 10000), // Duży bufor działa asynchronicznie
		received: 0,
	}
}

func (c *Client) sendAllMessages() {
	// W Go dobrą praktyką jest własny generator losowy dla współbieżnych procesów
	r := rand.New(rand.NewSource(time.Now().UnixNano() + int64(c.id)))
	count := 0

	for c.msgLeft > 0 {
		chosen := r.Intn(c.n)

		fmt.Printf("[CLIENT %d] #### SENT ### msg id %d\n", c.id, count)

		// Wysłanie wiadomości do synchronicznej kolejki serwera
		c.server.Queue <- Message{sourceID: c.id, messageID: count, receiverID: chosen}

		count++
		c.msgLeft--
	}

	// Informacja dla serwera, że już nie wysyłamy
	c.server.Queue <- Message{sourceID: c.id, messageID: -1, receiverID: -1}
	fmt.Printf("[CLIENT %d] #### NOT SENDING\n", c.id)
}

func (c *Client) Run(wg *sync.WaitGroup) {
	defer wg.Done() // Zgłoszenie zamknięcia klienta do WaitGroup

	// Uruchomienie wysyłania w tle (odpowiednik new Thread().start())
	go c.sendAllMessages()

	// Główny wątek zajmuje się tylko odbieraniem ze swojej skrzynki
	for {
		msg := <-c.Inbox // Pobranie ze skrzynki, blokuje jeśli pusta

		if msg.messageID == -999 { // Shutdown
			fmt.Printf("[CLIENT %d] #### SHUTDOWN RECEIVED\n", c.id)
			break
		}

		fmt.Printf("[CLIENT %d] #### RECEIVE ### msg id %d # FROM %d\n",
			c.id, msg.messageID, msg.sourceID)
		c.received++
	}

	fmt.Printf("[CLIENT %d] #### RECEIVED %d SIGNALS\n", c.id, c.received)
}