package com.philosophers.SimpleObjects;

import java.util.Optional;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;

public class Philosopher implements Runnable, Comparable<Philosopher> {
    public int id;
    public int numberOfTries;
    public int count;
    public int fail = 0;
    public States state = States.THINKING;
    public Side left;
    public Side right;
    BlockingQueue<Message> messageQueue = new java.util.concurrent.LinkedBlockingQueue<>();

    public Philosopher(int i, int n) {
        this.id = i;
        this.numberOfTries = n;
        this.count = 0;
    }

    private void send_request() throws InterruptedException {
        if(left.fork.isEmpty() || right.fork.isEmpty()) fail++;
        if(left.fork.isEmpty()) {
            left.neighbour.messageQueue.offer(new Message(Message_Type.REQUEST_FORK, this, null), 1000, TimeUnit.MILLISECONDS);
        }
        
        if(right.fork.isEmpty()) {
            right.neighbour.messageQueue.offer(new Message(Message_Type.REQUEST_FORK, this, null), 1000, TimeUnit.MILLISECONDS);
        }
    }
    
    private void handleMessage(Message message) throws InterruptedException {
        switch (message.type) {
            case REQUEST_FORK:
                if(message.source == left.neighbour) {
                    if(left.fork.isPresent() && left.fork.get().isDirty) {
                        left.fork.get().isDirty = false;
                        left.neighbour.messageQueue.offer(new Message(Message_Type.GIVE_FORK, this, left.fork.get()), 5000, TimeUnit.MILLISECONDS);
                        left.fork = Optional.empty();
                        
                        
                        if(this.state == States.WANTS_TO_EAT) {
                            left.neighbour.messageQueue.offer(new Message(Message_Type.REQUEST_FORK, this, null), 1000, TimeUnit.MILLISECONDS);
                        }
                    } else if (left.fork.isPresent()) {
                        left.pendingRequest = true;
                    }
                } else {
                    if(right.fork.isPresent() && right.fork.get().isDirty) {
                        right.fork.get().isDirty = false;
                        right.neighbour.messageQueue.offer(new Message(Message_Type.GIVE_FORK, this, right.fork.get()), 5000, TimeUnit.MILLISECONDS);
                        right.fork = Optional.empty();
                        
                        
                        if(this.state == States.WANTS_TO_EAT) {
                            right.neighbour.messageQueue.offer(new Message(Message_Type.REQUEST_FORK, this, null), 1000, TimeUnit.MILLISECONDS);
                        }
                    } else if (right.fork.isPresent()) {
                        right.pendingRequest = true;
                    }
                }
                break;

            case GIVE_FORK:
                if(message.source == left.neighbour) {
                    left.fork = message.fork;
                } else {
                    right.fork = message.fork;
                }
                break;
        
            default:
                break;
        }
    }

    @Override
    public int compareTo(Philosopher other) {
        if(other == null) {
            throw new NullPointerException("Cannot compare with null");
        }
        return Integer.compare(this.id, other.id);
    }

    @Override
    public void run() {
        try {
            while(!this.state.equals(States.DONE)) {
                switch (this.state) {
                    case THINKING:
                        System.out.println("Filozof "+id+" wszedł w stan THINKING");
                        long thinkTime = 100 + (long)(Math.random()*500);
                        long endTime = System.currentTimeMillis() + thinkTime;
                        while(System.currentTimeMillis() < endTime) {
                            long timeLeft = endTime - System.currentTimeMillis();
                            if(timeLeft < 0 ) break;

                            Message msg = messageQueue.poll(timeLeft, TimeUnit.MILLISECONDS);
                            if(msg != null) handleMessage(msg);
                        }
                        state = States.WANTS_TO_EAT;
                        break;
                
                    case WANTS_TO_EAT:
                        System.out.println("Filozof "+id+" wszedł w stan WANTS_TO_EAT");
                        send_request();

                        while(left.fork.isEmpty() || right.fork.isEmpty()) {
                            Message msg = messageQueue.take();
                            if(msg != null) handleMessage(msg);
                        }
                        this.state = States.EATS;
                        break;
                    
                    case EATS:
                        System.out.println("Filozof "+id+" wszedł w stan EATS");
                        long eatingTime = 200 + (long)(Math.random()*500);
                        Thread.sleep(eatingTime);
                        left.fork.get().isDirty = true;
                        right.fork.get().isDirty = true;
                        this.count++;
                        
                        
                        
                        if (left.pendingRequest && left.fork.isPresent()) {
                            left.fork.get().isDirty = false;
                            left.neighbour.messageQueue.offer(new Message(Message_Type.GIVE_FORK, this, left.fork.get()), 5000, TimeUnit.MILLISECONDS);
                            left.fork = Optional.empty();
                            left.pendingRequest = false;
                        }
                        if (right.pendingRequest && right.fork.isPresent()) {
                            right.fork.get().isDirty = false;
                            right.neighbour.messageQueue.offer(new Message(Message_Type.GIVE_FORK, this, right.fork.get()), 5000, TimeUnit.MILLISECONDS);
                            right.fork = Optional.empty();
                            right.pendingRequest = false; 
                        }

                        if(this.count == this.numberOfTries) this.state = States.DONE;
                        else this.state = States.THINKING;
                        break;
                
                    default:
                        break;
                }
            }

            System.err.println("####### FILOZOF "+id+" JEST DONE ####### LICZBA FAILOW: "+fail+" ##############");
            while(left.fork.isPresent() || right.fork.isPresent()) {
                Message msg = messageQueue.poll(1000, TimeUnit.MILLISECONDS);
                if (msg != null) handleMessage(msg);
                else break;
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}