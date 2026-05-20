package com.ex6;

import java.util.ArrayList;

import com.ex6.Objects.Client;
import com.ex6.Objects.Server;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        int n = 5;
        int m = 10;
        
        if(args.length>=1) n = Integer.parseInt(args[0]); 
        if(args.length>=2) m = Integer.parseInt(args[1]);
        ArrayList<Client> clients = new ArrayList<>();
        Server server = new Server(n ,clients);
        
        for(int i=0; i<n; i++) {
            clients.addLast(new Client(i, n, m, server));
        }
        new Thread(server).start();

        for(Client client: clients) {
            new Thread(client).start();
        }
    }
}
