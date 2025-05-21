package www.restapi_2025.Services;

import www.restapi_2025.Objects.Review;
import www.restapi_2025.Objects.User;
import www.restapi_2025.Objects.Game;
import www.restapi_2025.Objects.Rating;
import www.restapi_2025.Repositiories.ReviewRepository;
import www.restapi_2025.Repositiories.UserRepository;
import www.restapi_2025.Repositiories.GameRepository;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Optional;

@Service
public class ReviewService {

    private final ReviewRepository reviewRepository;
    private final UserRepository userRepository;
    private final GameRepository gameRepository;

    @Autowired
    public ReviewService(ReviewRepository reviewRepository, UserRepository userRepository, GameRepository gameRepository) {
        this.reviewRepository = reviewRepository;
        this.userRepository = userRepository;
        this.gameRepository = gameRepository;
    }




    @Transactional(readOnly = true)
    public List<Review> getAllReviews() {
        return reviewRepository.findAll();
    }

    @Transactional(readOnly = true)
    public Optional<Review> getReviewById(Long id) {
        return reviewRepository.findById(id);
    }



    @Transactional
    public Optional<Review> createReview(Long authorId, Long gameId, Review reviewDetails) {
        Optional<User> authorOptional = userRepository.findById(authorId);
        Optional<Game> gameOptional = gameRepository.findById(gameId);

        if (authorOptional.isPresent() && gameOptional.isPresent()) {
            User author = authorOptional.get();
            Game game = gameOptional.get();

            reviewDetails.setAuthor(author);
            reviewDetails.setGame(game);

            Review savedReview = reviewRepository.save(reviewDetails);
            return Optional.of(savedReview);
        } else {
            return Optional.empty();
        }
    }

    @Transactional
    public Optional<Review> updateReview(Long id, Review updatedReviewDetails) {
        return reviewRepository.findById(id)
                .map(review -> {
                    review.setRating(updatedReviewDetails.getRating());
                    review.setDescription(updatedReviewDetails.getDescription());

                    Review savedReview = reviewRepository.save(review);
                    return savedReview;
                });
    }


    @Transactional
    public boolean deleteReview(Long id) {
        if (reviewRepository.existsById(id)) {
            reviewRepository.deleteById(id);
            return true;
        }
        return false;
    }

    @Transactional(readOnly = true)
    public List<Review> getReviewsByAuthorId(Long authorId) {
        return reviewRepository.findByAuthorId(authorId);
    }

    @Transactional(readOnly = true)
    public List<Review> getReviewsByGameId(Long gameId) {
        return reviewRepository.findByGameId(gameId);
    }

}
