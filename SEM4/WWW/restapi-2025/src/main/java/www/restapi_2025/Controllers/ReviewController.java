package www.restapi_2025.Controllers;

import www.restapi_2025.Objects.Review;
import www.restapi_2025.Services.ReviewService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/reviews")
public class ReviewController {

    private final ReviewService reviewService;

    @Autowired
    public ReviewController(ReviewService reviewService) {
        this.reviewService = reviewService;
    }

    @GetMapping
    @PreAuthorize("isAuthenticated()")
    public ResponseEntity<List<Review>> getAllReviews() {
        List<Review> reviews = reviewService.getAllReviews();
        return new ResponseEntity<>(reviews, HttpStatus.OK);
    }


    @GetMapping("/{id}")
    @PreAuthorize("isAuthenticated()")
    public ResponseEntity<Review> getReviewById(@PathVariable Long id) {
        Optional<Review> review = reviewService.getReviewById(id);

        return review.map(ResponseEntity::ok)
                .orElseGet(() -> ResponseEntity.notFound().build());
    }


    @PostMapping("/user/{authorId}/game/{gameId}")
    @PreAuthorize("isAuthenticated()")
    public ResponseEntity<Review> createReview(
            @PathVariable Long authorId,
            @PathVariable Long gameId,
            @RequestBody Review reviewDetails
    ) {

        Optional<Review> createdReview = reviewService.createReview(authorId, gameId, reviewDetails);

        return createdReview.map(review -> new ResponseEntity<>(review, HttpStatus.CREATED))
                .orElseGet(() -> ResponseEntity.badRequest().build());
    }

    @PutMapping("/{id}")
    @PreAuthorize("isAuthenticated()")
    public ResponseEntity<Review> updateReview(@PathVariable Long id, @RequestBody Review updatedReviewDetails) {
        Optional<Review> updatedReview = reviewService.updateReview(id, updatedReviewDetails);

        return updatedReview.map(ResponseEntity::ok)
                .orElseGet(() -> ResponseEntity.notFound().build());
    }

    @DeleteMapping("/{id}")
    @PreAuthorize("hasRole('ROLE_ADMIN')")
    public ResponseEntity<Void> deleteReview(@PathVariable Long id) {
        boolean deleted = reviewService.deleteReview(id);

        if (deleted) {
            return ResponseEntity.noContent().build();
        } else {
            return ResponseEntity.notFound().build();
        }
    }

    @GetMapping("/by-author/{authorId}")
    @PreAuthorize("isAuthenticated()")
    public ResponseEntity<List<Review>> getReviewsByAuthor(@PathVariable Long authorId) {
        List<Review> reviews = reviewService.getReviewsByAuthorId(authorId);
        return new ResponseEntity<>(reviews, HttpStatus.OK);
    }

    @GetMapping("/by-game/{gameId}")
    @PreAuthorize("isAuthenticated()")
    public ResponseEntity<List<Review>> getReviewsByGame(@PathVariable Long gameId) {
        List<Review> reviews = reviewService.getReviewsByGameId(gameId);
        return new ResponseEntity<>(reviews, HttpStatus.OK);
    }

}
