package www.restapi_2025.Errors;

import lombok.AllArgsConstructor;
import lombok.Getter;

import java.util.Date;

@Getter
@AllArgsConstructor
class ErrorResponse {
    private int statusCode;
    private Date timestamp;
    private String message;
    private String description;
}