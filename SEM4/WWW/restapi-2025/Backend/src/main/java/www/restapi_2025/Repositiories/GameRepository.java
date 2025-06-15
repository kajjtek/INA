package www.restapi_2025.Repositiories;

import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import www.restapi_2025.Objects.Game;

import java.time.Year;
import java.util.List;

@Repository
public interface GameRepository extends JpaRepository<Game, Long> {

    Page<Game> findAll(Pageable pageable);

    Page<Game> findByNameIgnoreCase(String name, Pageable pageable);
    Page<Game> findByYearOfRelease(Year yearOfRelease, Pageable pageable);
    Page<Game> findByCreatorIgnoreCase(String creator, Pageable pageable);
    Page<Game> findByNameAndYearOfReleaseIgnoreCase(String name, Year year, Pageable pageable);
    Page<Game> findByNameAndCreatorIgnoreCase(String name, String creator, Pageable pageable);
    Page<Game> findByYearOfReleaseAndCreatorIgnoreCase(Year yearOfRelease, String creator, Pageable pageable);
    Page<Game> findByNameAndYearOfReleaseAndCreatorIgnoreCase(String name, Year yearOfRelease, String creator, Pageable pageable);
}
