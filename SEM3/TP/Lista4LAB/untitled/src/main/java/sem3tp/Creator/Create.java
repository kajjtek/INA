package sem3tp.Creator;

import sem3tp.Builder.BoardBuilder;
import sem3tp.Mover.Mover;
import sem3tp.Poles.InitPole;
import sem3tp.Poles.StandardPole;
import sem3tp.Storage.PoleStorage;
import sem3tp.Storage.UserStorage;
import sem3tp.User;

public interface Create {
    public StandardPole createSTDPole(int xCord, int yCord, int zCord);
    public InitPole createInitPole();
    public BoardBuilder createBoardBuilder(int x);
    public Mover createMover(int x);
    public PoleStorage createPoleStorage();
    public UserStorage createUserStorage();
    public User createUser(String username, String pwd);
}
