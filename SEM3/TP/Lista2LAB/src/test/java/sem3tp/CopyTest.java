package sem3tp;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

public class CopyTest {

    @Test
    public void testCreateCopy() {
        Copy copy = new Copy(0, 1);
        assertNotNull(copy);
        assertEquals(1, copy.id);
        assertEquals(1, copy.bookId);
        assertFalse(copy.isBorrowed);
    }
}

