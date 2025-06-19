package www.restapi_2025.Controllers;

import com.fasterxml.jackson.databind.deser.impl.JavaUtilCollectionsDeserializers;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;
import www.restapi_2025.JWT.JWTResponse;
import www.restapi_2025.JWT.JWTUtil;
import www.restapi_2025.JWT.LoginRequest;
import www.restapi_2025.Objects.User;
import www.restapi_2025.Services.UserService;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/auth")
public class AuthController {
    @Autowired
    AuthenticationManager authenticationManager;

    @Autowired
    UserService userService;

    @Autowired
    JWTUtil jwtUtil;

    @PostMapping("/signin")
    public ResponseEntity<?> authenticateUser(@RequestBody LoginRequest loginRequest){
        Authentication authentication = authenticationManager.authenticate(new UsernamePasswordAuthenticationToken(loginRequest.getUsername(), loginRequest.getPassword()));
        SecurityContextHolder.getContext().setAuthentication(authentication);

        String jwt = jwtUtil.generateJwtToken(authentication);

        User userDetails = (User) authentication.getPrincipal();

        List<String> roles = userDetails.getAuthorities().stream().map(item->item.getAuthority()).toList();

        return ResponseEntity.ok(new JWTResponse(jwt, userDetails.getId(), userDetails.getUsername(), userDetails.getRoles()));
    }

    @PostMapping("/signup")
    public ResponseEntity<?> registerUser(@RequestBody User user){
        if(userService.userExists(user.getUsername())){
            return ResponseEntity.badRequest().body("Error: Username is already taken");
        }
        User newUser = userService.createUser(user);

        return ResponseEntity.ok("User registered successfully!");
    }

    @PostMapping("/admin/create")
    @PreAuthorize("hasRole('ROLE_ADMIN')")
    public ResponseEntity<?> registerAdmin(@RequestBody User user){
        if(userService.userExists(user.getUsername())){
            return ResponseEntity.badRequest().body("Error: Username is already taken");
        }
        List<String> list = new ArrayList<>();
        list.add("ROLE_ADMIN");
        user.setRoles(list);
        User newUser = userService.createUser(user);

        return ResponseEntity.ok("User registered successfully!");
    }

    @DeleteMapping("/admin/delete/{id}")
    @PreAuthorize("hasRole('ROLE_ADMIN')")
    public ResponseEntity<?> deleteUser(@PathVariable Long id){
        boolean deleted = userService.deleteUser(id);

        if (deleted) {
            return ResponseEntity.noContent().build();
        } else {
            return ResponseEntity.notFound().build();
        }
    }
}
