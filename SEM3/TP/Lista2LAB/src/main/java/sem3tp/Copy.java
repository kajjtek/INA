package sem3tp;

public class Copy {
    public int id;
    public int bookId;
    public boolean isBorrowed;

    Copy(int numberOfCopies, int bookId){
        this.id=numberOfCopies+1;
        this.bookId=bookId;
        this.isBorrowed=false;
    }

    public void print(){
        System.out.println("Copy of a book: " + this.bookId + " of Id: " + this.id +". Is it borrowed: "+isBorrowed);
    }

    public void borrow(User user){
        this.isBorrowed=true;
        user.borrowedCopies.add(this);
    }
}
