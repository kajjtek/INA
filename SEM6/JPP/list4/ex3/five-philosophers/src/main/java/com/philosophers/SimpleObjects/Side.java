package com.philosophers.SimpleObjects;

import java.util.Optional;

public class Side {
    public Philosopher neighbour;
    public Optional<Fork> fork;
    public boolean pendingRequest = false; // DODANE: flaga odroczonego żądania

    public Side(Philosopher p1, Fork f) {
        this.neighbour = p1;
        this.fork = Optional.ofNullable(f); // Poprawione opakowanie w Optional
    }

    public Side(Philosopher p1) {
        this.neighbour = p1;
        this.fork = Optional.empty(); // Poprawiona składnia czyszczenia Optional
    }

    public Boolean isDirty() {
        // Poprawione wyciąganie wartości z Optional
        return fork.isPresent() && fork.get().isDirty;
    }
}