package com.philosophers.SimpleObjects;

import java.util.Optional;

enum Message_Type {
    GIVE_FORK, REQUEST_FORK
}

public class Message {
    public Message(Message_Type t, Philosopher s, Fork f) {
        this.type = t;
        this.source = s;
        this.fork = Optional.ofNullable(f);
    }
    public Message_Type type;
    public Philosopher source;
    public Optional<Fork> fork;
}
