package sem3tp;

import java.util.ArrayList;
import java.util.Scanner;

public class Library {
//    public ArrayList<Book> bookArrayList = new ArrayList<>();
//    public ArrayList<User> userArrayList = new ArrayList<>();

    public BookStorage bookStorage;
    public UserStorage userStorage;

    Library(){
        this.bookStorage=new BookStorage();
        this.userStorage=new UserStorage();
    }

    public Object findObject(String name, ArrayList<?> list){
            for (Object o : list) {
                if (o instanceof Book temp) {
                    if (temp.name.equals(name)) {
                        return temp;
                    }
                }
                if (o instanceof User temp) {
                    if (temp.username.equals(name)) {
                        return temp;
                    }
                }
            }
        return null;
    }

    public void showAllUsers(){
        userStorage.findAll();
//        for(User x: userArrayList){
//            System.out.println("User: "+x.username+" Id: "+x.id);
//        }
    }

    public void showAllBooks(){
        bookStorage.findAll();
    }

    public void showUser(String name){
        try {
            User temp = userStorage.findByName(name);
            if(temp==null)throw new Exception("User not found");
            temp.print();
        } catch (Exception e) {
            System.out.println(e.toString());
        }
    }

    public void showCopy(String name,int Id){
        try{
            Book temp = bookStorage.findByName(name);
            if (temp == null) {
                throw new Exception("The book does not exist");
            }
            temp.showCopy(Id);
        } catch (Exception e) {
            System.out.println(e.toString());
        }
    }

    public void showBook(String name){
        try{
            Book temp = bookStorage.findByName(name);
            if (temp == null) {
                throw new Exception("The book does not exist");
            }
        } catch (Exception e) {
            System.out.println(e.toString());
        }
    }

    public void createUser(String name){
        userStorage.add(name);
    }

    public void createBook(String name){
        bookStorage.add(name);
    }

    public void createCopy(String name){
        try {
            Book temp = bookStorage.findByName(name);
            if(temp==null)throw new Exception("Book not found");
            temp.createCopy();
        }catch (Exception e){
            System.out.println(e.toString());
        }
    }

    public void borrowCopy(String name, String username){
        try {
            User tempUser = userStorage.findByName(username);
            if(tempUser==null)throw new Exception("User not found");
            Book temp = bookStorage.findByName(name);
            if(temp==null)throw new Exception("Book not found");
            temp.borrow(tempUser);
        } catch (Exception e) {
            System.out.println(e.toString());
        }
    }
}
