package sem3tp;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

public class UserTest {

    @Test
    public void testCreateUser() {
        User user = new User(0, "Test User");
        assertNotNull(user);
        assertEquals(1, user.id);
        assertEquals("Test User", user.username);
    }

}

