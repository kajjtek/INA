package www.restapi_2025.JWT;

import lombok.Data;
import lombok.Getter;

@Data
public class LoginRequest {
    private String username;
    private String password;
}
