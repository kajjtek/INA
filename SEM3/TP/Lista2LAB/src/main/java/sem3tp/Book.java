package sem3tp;

import java.util.ArrayList;

public class Book {
    public int id;
    public String name;
    public CopyStorage copyStorage;

    Book(int numberOfBooks,String name){
        this.id=numberOfBooks+1;
        this.name=name;
        this.copyStorage=new CopyStorage();
    }

    public void print(){
        System.out.println("Book: " + this.name + " of Id: " + this.id);
    }

    public void borrow(User user){
        try {
            Copy temp = copyStorage.borrow();
            if(temp==null)throw new Exception("There is no copy that can be borrowed");
            temp.borrow(user);
        }catch (Exception e){
            System.out.println(e.toString());
        }
    }

    public void createCopy(){
        copyStorage.add(id);
    }

    public void showCopy(int Id){
        copyStorage.findById(Id).print();
    }
}
