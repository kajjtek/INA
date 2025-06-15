package www.restapi_2025.Services;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.stereotype.Service;
import www.restapi_2025.Objects.Game;
import www.restapi_2025.Repositiories.GameRepository;

import java.time.Year;
import java.util.List;
import java.util.Optional;

@Service
public class GameService {
    private final GameRepository gameRepository;

    @Autowired
    public GameService(GameRepository gameRepository){
        this.gameRepository=gameRepository;
    }

    //Filtering implemented
    public Page<Game> getAllGames(String name, Integer yearOfRelease, String creator, Pageable pageable){
        if(name!=null && yearOfRelease!=null && creator!=null){
            return gameRepository.findByNameAndYearOfReleaseAndCreatorIgnoreCase(name,Year.of(yearOfRelease),creator,pageable);
        }
        else if(name!=null && yearOfRelease!=null){
            return gameRepository.findByNameAndYearOfReleaseIgnoreCase(name,Year.of(yearOfRelease),pageable);
        }
        else if(name!=null && creator!=null){
            return gameRepository.findByNameAndCreatorIgnoreCase(name, creator,pageable);
        }
        else if(yearOfRelease!=null && creator!=null){
            return gameRepository.findByYearOfReleaseAndCreatorIgnoreCase(Year.of(yearOfRelease), creator,pageable);
        }
        else if(name!=null){
            return gameRepository.findByNameIgnoreCase(name,pageable);
        }
        else if(yearOfRelease!=null){
            return gameRepository.findByYearOfRelease(Year.of(yearOfRelease),pageable);
        } else if (creator!=null) {
            return gameRepository.findByCreatorIgnoreCase(creator,pageable);
        }
        return gameRepository.findAll(pageable);
    }

    public Optional<Game> getGameById(Long id){
        return gameRepository.findById(id);
    }

    public Game createGame(Game game){
        return gameRepository.save(game);
    }

    public Optional<Game> updateGame(Long id, Game updatedGame){
        return gameRepository.findById(id)
                .map(game -> {
                    game.setName(updatedGame.getName());
                    game.setYearOfRelease(updatedGame.getYearOfRelease());
                    game.setCreator(updatedGame.getCreator());
                    return gameRepository.save(game);
                });
    }

    public boolean deleteGame(Long id){
        if(gameRepository.existsById(id)){
            gameRepository.deleteById(id);
            return true;
        }
        return false; //not found
    }
}
