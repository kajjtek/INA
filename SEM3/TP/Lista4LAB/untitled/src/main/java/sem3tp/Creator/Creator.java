package sem3tp.Creator;

import sem3tp.Builder.BoardBuilder;
import sem3tp.Mover.Mover;
import sem3tp.Poles.InitPole;
import sem3tp.Poles.StandardPole;
import sem3tp.Storage.PoleStorage;
import sem3tp.Storage.UserStorage;
import sem3tp.User;

public class Creator implements Create{

    private static Creator instance;

    private Creator(){}

    public static Creator getInstance() {
        if(instance==null){
            synchronized (Creator.class) {
                if(instance==null){
                    instance= new Creator();
                }
            }
        }
        return instance;
    }

    @Override
    public StandardPole createSTDPole(int xCord, int yCord, int zCord) {
        return new StandardPole(xCord, yCord, zCord);
    }

    @Override
    public InitPole createInitPole() {
        return new InitPole();
    }

    @Override
    public BoardBuilder createBoardBuilder(int x) {
        return new BoardBuilder(x);
    }

    @Override
    public Mover createMover(int x) {
        return Mover.getInstance(x);
    }

    @Override
    public PoleStorage createPoleStorage() {
        return new PoleStorage();
    }

    @Override
    public UserStorage createUserStorage() {
        return new UserStorage();
    }

    @Override
    public User createUser(String username, String pwd) {
        return new User(username,pwd);
    }
}
