package sem3tp;

import java.util.ArrayList;
//Implementacja Storage dla Book
public class BookStorage implements Storage<Book>{
    private final ArrayList<Book> bookArrayList;

    BookStorage(){
        this.bookArrayList = new ArrayList<>();
    }

    @Override
    public void add(String name){
        bookArrayList.add(new Book(bookArrayList.size(),name));
    }

    @Override
    public Book findByName(String name){
        for(Book book:bookArrayList){
            if(book.name.equals(name)){
                return book;
            }
        }
        return null;
    }

    @Override
    public void findAll() {
     for(Book book:bookArrayList){
         book.print();
     }
    }
}
