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
	Inbox    chan Message 
	received int
}

func NewClient(id, n, messages int, server *Server) *Client {
	return &Client{
		id:       id,
		n:        n,
		msgLeft:  messages,
		server:   server,
		Inbox:    make(chan Message, 10000), 
		received: 0,
	}
}

func (c *Client) sendAllMessages() {
	
	r := rand.New(rand.NewSource(time.Now().UnixNano() + int64(c.id)))
	count := 0

	for c.msgLeft > 0 {
		chosen := r.Intn(c.n)

		fmt.Printf("[CLIENT %d] #### SENT ### msg id %d\n", c.id, count)

		
		c.server.Queue <- Message{sourceID: c.id, messageID: count, receiverID: chosen}

		count++
		c.msgLeft--
	}

	
	c.server.Queue <- Message{sourceID: c.id, messageID: -1, receiverID: -1}
	fmt.Printf("[CLIENT %d] #### NOT SENDING\n", c.id)
}

func (c *Client) Run(wg *sync.WaitGroup) {
	defer wg.Done() 

	
	go c.sendAllMessages()

	
	for {
		msg := <-c.Inbox 

		if msg.messageID == -999 { 
			fmt.Printf("[CLIENT %d] #### SHUTDOWN RECEIVED\n", c.id)
			break
		}

		fmt.Printf("[CLIENT %d] #### RECEIVE ### msg id %d # FROM %d\n",
			c.id, msg.messageID, msg.sourceID)
		c.received++
	}

	fmt.Printf("[CLIENT %d] #### RECEIVED %d SIGNALS\n", c.id, c.received)
}