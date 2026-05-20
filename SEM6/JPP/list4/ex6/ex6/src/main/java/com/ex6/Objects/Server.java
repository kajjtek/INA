package com.ex6.Objects;

import java.util.ArrayList;
import java.util.concurrent.SynchronousQueue;

public class Server implements Runnable {
    SynchronousQueue<Message> queue = new SynchronousQueue<>(true);
    ArrayList<Client> clients;
    int sending_clients;

    public Server(int n, ArrayList<Client> cl) {
        this.clients = cl;
        sending_clients = n;
    }

    private void send(Message msg, Client client) {
        try {
            client.inbox.put(msg);
            System.out.println("[SEVER] SENDING MESSAGE: "+msg.message_id()+" FROM: "+msg.source_id()+" TO: "+msg.receiver_id());
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    private void receive(Message msg) {
        try {
            if(msg==null) return;
            System.out.println("[SERVER] RECEIVED MESSAGE: "+msg.message_id()+" FROM: "+msg.source_id()+" TO: "+msg.receiver_id());
            if(msg.message_id() == -1) sending_clients--;
            else {
                Client client = clients.get(msg.receiver_id());
                client.inbox.put(msg);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    @Override
    public void run() {
        while(sending_clients>0) {
            Message msg = queue.poll();

            receive(msg);
        }
        for(Client client: clients) {
            send(new Message(-1, -999, client.id), client);
        }
        System.out.println("[SEVER] shutting down");
    }
}