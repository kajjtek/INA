package sem3tp;


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
        copyStorage.borrow(user);
    }

    public void createCopy(){
        copyStorage.add(id);
    }

    public void showCopy(int Id){
        copyStorage.findById(Id).print();
    }
}
