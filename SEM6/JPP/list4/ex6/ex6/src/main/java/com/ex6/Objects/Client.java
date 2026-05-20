package com.ex6.Objects;

import java.util.Random;
import java.util.concurrent.BlockingDeque;
import java.util.concurrent.BlockingQueue;

public class Client implements Runnable {
    
    public int msg_left;
    public int id;
    public int n;
    public Server server;
    public BlockingQueue<Message> inbox = new java.util.concurrent.LinkedBlockingQueue<>();
    private int received = 0;

    public Client(int i, int number_of_clients, int number_of_messages, Server s) {
        this.id=i;
        this.n = number_of_clients;
        this.msg_left = number_of_messages;
        this.server = s;
    }

    public void receive(Message msg) {
        System.out.println("[CLIENT "+id+"] #### RECEIVE ### msg id "+msg.message_id()+" # FROM "+msg.source_id());
        received++;
    }

    private void send(int message_id, int receiver) {
        try {
            Message msg = new Message(this.id, message_id, receiver);
            System.out.println("[CLIENT "+id+"] #### SENT ### msg id "+message_id);
            server.queue.put(msg);
            msg_left--;
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

    }
    private void sendAllMessages() {
        try {
            int count = 0;
            Random random = new Random();
            while(msg_left>0) {
                int chosen = random.nextInt(n);

                send(count++, chosen);
            }
            server.queue.put(new Message(id, -1, -1)); //informacja ze juz nie wysylamy
            System.out.println("[CLIENT "+id+"] #### NOT SENDING");
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    @Override
    public void run() {
        new Thread(this::sendAllMessages).start();
        try {
            while(true) {
                Message msg = inbox.take();
                
                if (msg.message_id() == -999) { //shutdown
                    System.out.println("[CLIENT "+id+"] #### SHUTDOWN RECEIVED");
                    break;
                } else {
                    receive(msg);
                }
            }
            System.out.println("[CLIENT "+id+"] #### RECEIVED "+received+" SIGNALS");
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

    }
}
