package main

import (
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"sync"
	"time"
)


type State int

const (
	Thinking State = iota
	WantsToEat
	Eating
	Done
)

type MessageKind int

const (
	GiveFork MessageKind = iota
	RequestFork
)


type Fork struct {
	IsDirty bool
}

type Message struct {
	Kind     MessageKind
	SourceID int
	Fk       *Fork
}

type Side struct {
	NeighbourChan  chan Message
	NeighbourID    int
	Fk             *Fork
	PendingRequest bool
}

type Philosopher struct {
	ID         int
	MaxMeals   int
	CountMeals int
	Fail       int
	State      State
	Left       Side
	Right      Side
	Mailbox    chan Message
}


func (p *Philosopher) sendRequest() {
	if p.Left.Fk == nil || p.Right.Fk == nil {
		p.Fail++
	}

	if p.Left.Fk == nil {
		select {
		case p.Left.NeighbourChan <- Message{Kind: RequestFork, SourceID: p.ID}:
		case <-time.After(1 * time.Second):
		}
	}

	if p.Right.Fk == nil {
		select {
		case p.Right.NeighbourChan <- Message{Kind: RequestFork, SourceID: p.ID}:
		case <-time.After(1 * time.Second):
		}
	}
}


func (p *Philosopher) handleMessage(msg Message) {
	switch msg.Kind {
	case RequestFork:
		if msg.SourceID == p.Left.NeighbourID {
			if p.Left.Fk != nil && p.Left.Fk.IsDirty {
				p.Left.Fk.IsDirty = false
				select {
				case p.Left.NeighbourChan <- Message{Kind: GiveFork, SourceID: p.ID, Fk: p.Left.Fk}:
				case <-time.After(5 * time.Second):
				}
				p.Left.Fk = nil

				if p.State == WantsToEat {
					select {
					case p.Left.NeighbourChan <- Message{Kind: RequestFork, SourceID: p.ID}:
					case <-time.After(1 * time.Second):
					}
				}
			} else if p.Left.Fk != nil {
				p.Left.PendingRequest = true
			}
		} else {
			if p.Right.Fk != nil && p.Right.Fk.IsDirty {
				p.Right.Fk.IsDirty = false
				select {
				case p.Right.NeighbourChan <- Message{Kind: GiveFork, SourceID: p.ID, Fk: p.Right.Fk}:
				case <-time.After(5 * time.Second):
				}
				p.Right.Fk = nil

				if p.State == WantsToEat {
					select {
					case p.Right.NeighbourChan <- Message{Kind: RequestFork, SourceID: p.ID}:
					case <-time.After(1 * time.Second):
					}
				}
			} else if p.Right.Fk != nil {
				p.Right.PendingRequest = true
			}
		}

	case GiveFork:
		if msg.SourceID == p.Left.NeighbourID {
			p.Left.Fk = msg.Fk
		} else {
			p.Right.Fk = msg.Fk
		}
	}
}


func (p *Philosopher) Run(wg *sync.WaitGroup) {
	defer wg.Done()
	
	r := rand.New(rand.NewSource(time.Now().UnixNano() + int64(p.ID)))

	for p.State != Done {
		switch p.State {
		case Thinking:
			fmt.Printf("Filozof %d wszedl w stan THINKING\n", p.ID)
			thinkMs := 100 + r.Intn(500)
			endTime := time.Now().Add(time.Duration(thinkMs) * time.Millisecond)

			
			for time.Now().Before(endTime) {
				timeLeft := time.Until(endTime)
				if timeLeft <= 0 {
					break
				}
				select {
				case msg := <-p.Mailbox:
					p.handleMessage(msg)
				case <-time.After(timeLeft):
				}
			}
			p.State = WantsToEat

		case WantsToEat:
			fmt.Printf("Filozof %d wszedl w stan WANTS_TO_EAT\n", p.ID)
			p.sendRequest()

			
			for p.Left.Fk == nil || p.Right.Fk == nil {
				msg := <-p.Mailbox
				p.handleMessage(msg)
			}
			p.State = Eating

		case Eating:
			fmt.Printf("Filozof %d wszedl w stan EATS\n", p.ID)
			eatMs := 200 + r.Intn(500)
			time.Sleep(time.Duration(eatMs) * time.Millisecond)

			p.Left.Fk.IsDirty = true
			p.Right.Fk.IsDirty = true
			p.CountMeals++

			
			if p.Left.PendingRequest && p.Left.Fk != nil {
				p.Left.Fk.IsDirty = false
				select {
				case p.Left.NeighbourChan <- Message{Kind: GiveFork, SourceID: p.ID, Fk: p.Left.Fk}:
				case <-time.After(5 * time.Second):
				}
				p.Left.Fk = nil
				p.Left.PendingRequest = false
			}

			if p.Right.PendingRequest && p.Right.Fk != nil {
				p.Right.Fk.IsDirty = false
				select {
				case p.Right.NeighbourChan <- Message{Kind: GiveFork, SourceID: p.ID, Fk: p.Right.Fk}:
				case <-time.After(5 * time.Second):
				}
				p.Right.Fk = nil
				p.Right.PendingRequest = false
			}

			if p.CountMeals == p.MaxMeals {
				p.State = Done
			} else {
				p.State = Thinking
			}
		}
	}

	fmt.Fprintf(os.Stderr, "####### FILOZOF %d JEST DONE ####### LICZBA FAILOW: %d ##############\n", p.ID, p.Fail)

	
	for p.Left.Fk != nil || p.Right.Fk != nil {
		select {
		case msg := <-p.Mailbox:
			p.handleMessage(msg)
		case <-time.After(1 * time.Second):
			return
		}
	}
}

func main() {
	numPhilosophers := 5
	numDishes := 10
	var err error

	if len(os.Args) >= 2 {
		numPhilosophers, err = strconv.Atoi(os.Args[1])
		if err != nil {
			fmt.Println("Błąd parsowania liczby filozofów:", err)
			return
		}
	}
	if len(os.Args) >= 3 {
		numDishes, err = strconv.Atoi(os.Args[2])
		if err != nil {
			fmt.Println("Błąd parsowania liczby posiłków:", err)
			return
		}
	}

	
	philosophers := make([]*Philosopher, numPhilosophers)
	for i := 0; i < numPhilosophers; i++ {
		philosophers[i] = &Philosopher{
			ID:       i,
			MaxMeals: numDishes,
			State:    Thinking,
			Mailbox:  make(chan Message, 50),
		}
	}

	
	for i := 0; i < numPhilosophers; i++ {
		leftIdx := (i - 1 + numPhilosophers) % numPhilosophers
		rightIdx := (i + 1) % numPhilosophers

		philosophers[i].Left.NeighbourChan = philosophers[leftIdx].Mailbox
		philosophers[i].Left.NeighbourID = leftIdx

		philosophers[i].Right.NeighbourChan = philosophers[rightIdx].Mailbox
		philosophers[i].Right.NeighbourID = rightIdx
	}

	
	forks := make([]*Fork, numPhilosophers)
	for i := 0; i < numPhilosophers; i++ {
		forks[i] = &Fork{IsDirty: true}
	}

	for i := 0; i < numPhilosophers; i++ {
		p1 := philosophers[i]
		p2 := philosophers[(i+1)%numPhilosophers]

		if p1.ID < p2.ID {
			p1.Right.Fk = forks[i]
			p2.Left.Fk = nil
		} else {
			p1.Right.Fk = nil
			p2.Left.Fk = forks[i]
		}
	}

	
	var wg sync.WaitGroup
	for i := 0; i < numPhilosophers; i++ {
		wg.Add(1)
		go philosophers[i].Run(&wg)
	}

	wg.Wait()
}