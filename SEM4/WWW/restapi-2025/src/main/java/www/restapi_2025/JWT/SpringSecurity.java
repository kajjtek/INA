package www.restapi_2025.JWT;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.http.HttpMethod;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.dao.DaoAuthenticationProvider;
import org.springframework.security.config.annotation.authentication.configuration.AuthenticationConfiguration;
import org.springframework.security.config.annotation.authentication.configuration.EnableGlobalAuthentication;
import org.springframework.security.config.annotation.method.configuration.EnableGlobalMethodSecurity;
import org.springframework.security.config.annotation.method.configuration.EnableMethodSecurity;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.http.SessionCreationPolicy;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;
import www.restapi_2025.Services.UserService;

import org.springframework.web.cors.CorsConfiguration;
import org.springframework.web.cors.UrlBasedCorsConfigurationSource;
import org.springframework.web.filter.CorsFilter;
import org.springframework.security.config.Customizer;

@Configuration
@EnableMethodSecurity(prePostEnabled = true)
public class SpringSecurity {
//    @Autowired
//    private UserService userService;

//    @Autowired
//    private JWTRequestFilter jwtRequestFilter;

    @Bean
    public PasswordEncoder passwordEncoder(){
        return new BCryptPasswordEncoder();
    }

    @Bean
    public AuthenticationManager authenticationManager(AuthenticationConfiguration authenticationConfiguration) throws Exception {
        return authenticationConfiguration.getAuthenticationManager();
    }

    @Bean
    public DaoAuthenticationProvider authenticationProvider(UserService userService) {
        DaoAuthenticationProvider authProvider = new DaoAuthenticationProvider();
        authProvider.setUserDetailsService(userService);
        authProvider.setPasswordEncoder(passwordEncoder());
        return authProvider;
    }

    @Bean
    public CorsFilter corsFilter() {
        UrlBasedCorsConfigurationSource source = new UrlBasedCorsConfigurationSource();
        CorsConfiguration config = new CorsConfiguration();
        config.setAllowCredentials(true); // Allow credentials (e.g., Authorization header)
        // Set the allowed origin to your frontend's URL.
        // If your frontend runs on a different port (e.g., 5174), change it here.
        config.addAllowedOrigin("http://localhost:5173"); // <-- Frontend URL
        config.addAllowedHeader("*"); // Allow all headers
        config.addAllowedMethod("*"); // Allow all HTTP methods (GET, POST, PUT, DELETE, OPTIONS)
        source.registerCorsConfiguration("/**", config); // Apply this CORS config to all paths
        return new CorsFilter(source);
    }

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http, DaoAuthenticationProvider authenticationProvider, JWTRequestFilter jwtRequestFilter) throws Exception {
        http.csrf(csrf -> csrf.disable())
                .cors(Customizer.withDefaults())
                .sessionManagement(session -> session.sessionCreationPolicy(SessionCreationPolicy.STATELESS))
                .authorizeHttpRequests(auth -> auth
                        .requestMatchers("/auth/**").permitAll()
                        .requestMatchers("/games/**").authenticated()
                        .requestMatchers(HttpMethod.GET, "/users/**").hasRole("ADMIN")
                        .requestMatchers(HttpMethod.POST, "/games").hasRole("ADMIN") // Assuming "ADMIN" role, adjust if needed
                        .requestMatchers(HttpMethod.POST, "/users").hasRole("ADMIN")
                        .requestMatchers(HttpMethod.POST, "/reviews").hasRole("ADMIN")
                        .requestMatchers(HttpMethod.DELETE, "/reviews").hasRole("ADMIN")
                        .requestMatchers(HttpMethod.DELETE, "/users").hasRole("ADMIN")
                        .requestMatchers(HttpMethod.DELETE, "/games").hasRole("ADMIN")
                        .requestMatchers(HttpMethod.PUT, "/reviews").hasRole("ADMIN")
                        .requestMatchers(HttpMethod.PUT, "/users").hasRole("ADMIN")
                        .requestMatchers(HttpMethod.PUT, "/games").hasRole("ADMIN")
                        .anyRequest().authenticated()
                );


        http.authenticationProvider(authenticationProvider);
        http.addFilterBefore(jwtRequestFilter, UsernamePasswordAuthenticationFilter.class);

        return http.build();
    }
}
