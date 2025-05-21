package www.restapi_2025.Repositiories;

import www.restapi_2025.Objects.Review;
import www.restapi_2025.Objects.Rating;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface ReviewRepository extends JpaRepository<Review, Long> {

    List<Review> findByAuthorId(Long authorId);

    List<Review> findByGameId(Long gameId);

    List<Review> findByRating(Rating rating);

    List<Review> findByGameIdAndRating(Long gameId, Rating rating);

    List<Review> findByAuthorIdAndGameId(Long authorId, Long gameId);

}
