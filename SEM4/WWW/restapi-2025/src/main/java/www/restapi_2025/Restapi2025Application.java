package www.restapi_2025;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import www.restapi_2025.Errors.GlobalExceptionHandler;

@SpringBootApplication
public class Restapi2025Application {

	public static void main(String[] args) {
		SpringApplication.run(Restapi2025Application.class, args);
	}

}
