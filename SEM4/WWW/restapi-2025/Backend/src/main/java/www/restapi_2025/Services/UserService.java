package www.restapi_2025.Services;


import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import www.restapi_2025.Objects.User;
import www.restapi_2025.Repositiories.UserRepository;

import java.util.List;
import java.util.Optional;

@Service
public class UserService implements UserDetailsService {
    private final UserRepository userRepository;
    private final PasswordEncoder passwordEncoder;
    
    @Autowired
    public UserService(UserRepository userRepository, PasswordEncoder passwordEncoder){
        this.userRepository=userRepository;
        this.passwordEncoder=passwordEncoder;
    }


    @Override
    @Transactional(readOnly=true)
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        return userRepository.findByUsername(username).orElseThrow(() -> new UsernameNotFoundException("User not found with username: " + username));
    }

    public boolean userExists(String username){
        return userRepository.findByUsername(username).isPresent();
    }

    @Transactional
    public User createUser(User user){
        user.setPassword(passwordEncoder.encode(user.getPassword()));
        return userRepository.save(user);
    }

    public List<User> getAllUsers(){
        return userRepository.findAll();
    }

    public Optional<User> getUserById(Long id){
        return userRepository.findById(id);
    }

    public Optional<User> updateUser(User updatedUserDetails){
        return userRepository.findById(updatedUserDetails.getId()).map(
                user -> {
                    user.setUsername(updatedUserDetails.getUsername());
                    user.setRoles(updatedUserDetails.getRoles());
                    user.setDescription(updatedUserDetails.getDescription());
                    user.setBirthDate(updatedUserDetails.getBirthDate());
                    user.setPassword(passwordEncoder.encode(updatedUserDetails.getPassword()));
                    return userRepository.save(user);
                }
        );
    }

    public boolean deleteUser(Long id){
        if(userRepository.existsById(id)){
            userRepository.deleteById(id);
            return true;
        }
        return false; //not found
    }
}
