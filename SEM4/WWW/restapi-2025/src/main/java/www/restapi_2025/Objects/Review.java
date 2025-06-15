package www.restapi_2025.Objects;

import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;

@Entity
@Table(name="reviews")
@Data
@NoArgsConstructor
@AllArgsConstructor
@EqualsAndHashCode(onlyExplicitlyIncluded = true)
public class Review {

    @Id
    @EqualsAndHashCode.Include
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Enumerated(EnumType.STRING)
    private Rating rating;
    private String description;

    @ManyToOne
    @JoinColumn(name="author_id", nullable = false)
    private User author;

    @ManyToOne
    @JoinColumn(name="game_id", nullable = false)
    private Game game;
}
