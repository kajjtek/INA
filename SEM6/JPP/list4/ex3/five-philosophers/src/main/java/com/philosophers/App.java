package com.philosophers;

import java.util.ArrayList;
import java.util.Optional;

import com.philosophers.SimpleObjects.Fork;
import com.philosophers.SimpleObjects.Philosopher;
import com.philosophers.SimpleObjects.Side;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        int num_of_philosophers = 5;
        int num_of_dishes = 10;
        
        if(args.length>=1) num_of_philosophers = Integer.parseInt(args[0]); 
        if(args.length>=2) num_of_dishes = Integer.parseInt(args[1]);


        ArrayList<Philosopher> philosophers = new ArrayList<>();
        for (int i = 0; i < num_of_philosophers; i++) {
            philosophers.add(new Philosopher(i, num_of_dishes));
        }

        for (int i = 0; i < num_of_philosophers; i++) {
            Philosopher current = philosophers.get(i);
            Philosopher leftNeighbour = philosophers.get((i - 1 + num_of_philosophers) % num_of_philosophers);
            Philosopher rightNeighbour = philosophers.get((i + 1) % num_of_philosophers);
            
            current.left = new Side(leftNeighbour);
            current.right = new Side(rightNeighbour);
        }

        for (int i = 0; i < num_of_philosophers; i++) {
            Fork fork = new Fork(); 
            Philosopher p1 = philosophers.get(i);
            Philosopher p2 = philosophers.get((i + 1) % num_of_philosophers);

            if (p1.id < p2.id) {
                p1.right.fork = Optional.of(fork);
                p2.left.fork = Optional.empty();
            } else {
                p1.right.fork = Optional.empty();
                p2.left.fork = Optional.of(fork);
            }
        }

        for (Philosopher p : philosophers) {
            new Thread(p).start();
        }
    }
}
