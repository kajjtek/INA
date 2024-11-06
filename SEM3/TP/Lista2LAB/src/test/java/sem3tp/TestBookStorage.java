package sem3tp;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

public class TestBookStorage {


    @Test
    public void testFindBookByName() {
        BookStorage bookStorage = new BookStorage();
        bookStorage.add("Test Book");
        Book book = bookStorage.findByName("Test Book");
        assertNotNull(book);
        assertEquals("Test Book", book.name);
    }

    @Test
    public void testFindAllBooks() {
        BookStorage bookStorage = new BookStorage();
        bookStorage.add("Test Book");
        bookStorage.findAll(); // Should print out all books
    }
}

