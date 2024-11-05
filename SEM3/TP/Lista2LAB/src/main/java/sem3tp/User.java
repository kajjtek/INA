package sem3tp;

import java.util.ArrayList;

public class User {
    public int id;
    public String username;
    public ArrayList<Copy> borrowedCopies;

    User(int numberOfUsers,String name){
        this.id=numberOfUsers+1;
        this.username=name;
        this.borrowedCopies=new ArrayList<Copy>();
    }

    public void print(){
        System.out.println("User: " + this.username + " of Id: " + this.id);
    }
}
