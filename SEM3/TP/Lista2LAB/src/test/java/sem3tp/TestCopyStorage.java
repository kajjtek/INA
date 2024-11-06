package sem3tp;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

public class TestCopyStorage {


    @Test
    public void testBorrowCopy() {
        CopyStorage copyStorage = new CopyStorage();
        copyStorage.add(1);
        User user = new User(0, "Test User");
        Book book = new Book(0,"test");
        book.createCopy();
        book.borrow(user);
        assertNotNull(book.copyStorage);
        assertTrue(book.copyStorage.findById(1).isBorrowed);
    }

    @Test
    public void testFindCopyById() {
        CopyStorage copyStorage = new CopyStorage();
        copyStorage.add(1);
        Copy copy = copyStorage.findById(1);
        assertNotNull(copy);
        assertEquals(1, copy.id);
    }
}

