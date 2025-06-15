package www.restapi_2025.Controllers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.web.bind.annotation.*;
import www.restapi_2025.Objects.Game;
import www.restapi_2025.Repositiories.GameRepository;
import www.restapi_2025.Services.GameService;

import java.time.Year;
import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/games")
public class GameController {
    private final GameService gameService;

    @Autowired
    public GameController(GameService gameService){
        this.gameService=gameService;
    }

    @GetMapping
    @PreAuthorize("isAuthenticated()")
    public ResponseEntity<Page<Game>> getAllGames(
            @RequestParam(required = false) Integer yearOfRelease,
            @RequestParam(required = false) String name,
            @RequestParam(required = false) String creator,
            Pageable pageable
    ) {
        Page<Game> allGames = gameService.getAllGames(name, yearOfRelease, creator, pageable);
        return new ResponseEntity<>(allGames, HttpStatus.OK);
    }

    @GetMapping("/{id}")
    @PreAuthorize("isAuthenticated()")
    public ResponseEntity<Game> getGameById(@PathVariable Long id){
        Optional<Game> game = gameService.getGameById(id);

        return game.map(ResponseEntity::ok).orElseGet(()->ResponseEntity.notFound().build());
        // maps the game object on responseentity.ok(object) function - that creates responseentity objrct with 200 and
        //game if it is successfull or else optional is empty
    }

    @PostMapping
    @PreAuthorize("hasRole('ROLE_ADMIN')")
    public ResponseEntity<Game> createGame(@RequestBody Game game){
        Game createdGame = gameService.createGame(game);
        return new ResponseEntity<>(createdGame, HttpStatus.CREATED);
    }

    @PutMapping("/{id}")
    @PreAuthorize("hasRole('ROLE_ADMIN')")
    public ResponseEntity<Game> updateGame(@PathVariable Long id, @RequestBody Game updatedGame){
        Optional<Game> game = gameService.updateGame(id, updatedGame);

        return game.map(ResponseEntity::ok).orElseGet(()->ResponseEntity.notFound().build());
    }

    @DeleteMapping("/{id}")
    @PreAuthorize("hasRole('ROLE_ADMIN')")
    public ResponseEntity<Game> deleteGame(@PathVariable Long id){
        boolean isDeleted = gameService.deleteGame(id);

        if(isDeleted) return ResponseEntity.noContent().build();
        return ResponseEntity.notFound().build();
    }

    @ExceptionHandler(IllegalArgumentException.class)
    @ResponseStatus(HttpStatus.BAD_REQUEST)
    public String handleIllegalArgumentException(IllegalArgumentException e){
        return e.getMessage();
    }
}
