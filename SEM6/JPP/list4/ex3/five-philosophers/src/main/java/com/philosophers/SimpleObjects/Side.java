package com.philosophers.SimpleObjects;

import java.util.Optional;

public class Side {
    public Philosopher neighbour;
    public Optional<Fork> fork;
    public boolean pendingRequest = false; 

    public Side(Philosopher p1, Fork f) {
        this.neighbour = p1;
        this.fork = Optional.ofNullable(f); 
    }

    public Side(Philosopher p1) {
        this.neighbour = p1;
        this.fork = Optional.empty(); 
    }

    public Boolean isDirty() {
        
        return fork.isPresent() && fork.get().isDirty;
    }
}