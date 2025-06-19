// src/main/java/www/restapi_2025/Controllers/UserController.java

package www.restapi_2025.Controllers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import www.restapi_2025.Objects.User;
import www.restapi_2025.Services.UserService;

import java.util.List;

@RestController
@RequestMapping("/users") // Dodaj RequestMapping dla ścieżki /users
public class UserController {

    private final UserService userService;

    @Autowired
    public UserController(UserService userService) {
        this.userService = userService;
    }

    @GetMapping // Endpoint GET /users
    @PreAuthorize("hasRole('ROLE_ADMIN')") // Dostęp tylko dla administratorów
    public ResponseEntity<List<User>> getAllUsers() {
        List<User> users = userService.getAllUsers();
        return new ResponseEntity<>(users, HttpStatus.OK);
    }
}
