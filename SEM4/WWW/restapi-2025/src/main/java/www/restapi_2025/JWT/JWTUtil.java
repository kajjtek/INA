package www.restapi_2025.JWT;

import io.jsonwebtoken.*; // Importuj klasy z biblioteki JJWT
import io.jsonwebtoken.security.Keys; // Do generowania kluczy
import jakarta.annotation.PostConstruct;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.stereotype.Component;

import javax.crypto.SecretKey;
import java.util.Date;

@Component
public class JWTUtil {
    private static final Logger logger = LoggerFactory.getLogger(JWTUtil.class);

    @Value("${app.jwtSecret}")
    private String jwtSecret;

    @Value("${app.jwtExpirationMs}")
    private int jwtExpirationMs;

    private SecretKey key;

    @PostConstruct
    public void init(){
        this.key=Keys.hmacShaKeyFor(jwtSecret.getBytes());
    }
//authentication holds the result of authentication process - if it is correct then hellyeah else it doesnt execute
    public String generateJwtToken(Authentication authentication){
        UserDetails userPrincipial = (UserDetails) authentication.getPrincipal();

        return Jwts.builder()
                .setSubject(userPrincipial.getUsername())
                .setIssuedAt(new Date())
                .setExpiration(new Date(new Date().getTime()+jwtExpirationMs))
                .signWith(key,SignatureAlgorithm.HS512)//signing assures that the authentication was made by a verified author
                .compact();//token musi byc na tyle dlugi zeby nie dalo sie go zbrutforcowac w czasie expiration ms
    }

    public String getUsernameFromToken(String authToken){
        return Jwts.parserBuilder().setSigningKey(key).build().parseClaimsJws(authToken).getBody().getSubject();
    }

    public boolean validateJwtToken(String authToken){
        try{
            Jwts.parserBuilder().setSigningKey(key).build().parseClaimsJws(authToken);
            return true;
        }catch (SignatureException e) {
            logger.error("Invalid JWT signature: {}", e.getMessage());
        } catch (MalformedJwtException e) {
            logger.error("Invalid JWT token: {}", e.getMessage());
        } catch (ExpiredJwtException e) {
            logger.error("JWT token is expired: {}", e.getMessage());
        } catch (UnsupportedJwtException e) {
            logger.error("JWT token is unsupported: {}", e.getMessage());
        } catch (IllegalArgumentException e) {
            logger.error("JWT claims string is empty: {}", e.getMessage());
        }
        return false;
    }


}
