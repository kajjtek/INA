package sem3tp;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class BookTest {

    @Test
    void TestCreateBook(){
        Book test = new Book(1,"test");
        assertNotNull(test);
        assertEquals("test",test.name);
    }

    @Test
    void TestCreateCopy(){
        Book test = new Book(1,"test");
        test.createCopy();
        assertNotNull(test.copyStorage);
    }

    @Test
    public void testBorrowBook() {
        Book book = new Book(1, "Test Book");
        User user = new User(1, "Test User");
        book.borrow(user);
        assertNotNull(user.borrowedCopies);
    }

}
