package sem3tp;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

public class TestUserStorage {

    @Test
    public void testFindUserByName() {
        UserStorage userStorage = new UserStorage();
        userStorage.add("Test User");
        User user = userStorage.findByName("Test User");
        assertNotNull(user);
        assertEquals("Test User", user.username);
    }

    @Test
    public void testFindAllUsers() {
        UserStorage userStorage = new UserStorage();
        userStorage.add("Test User");
        userStorage.findAll();
    }
}

