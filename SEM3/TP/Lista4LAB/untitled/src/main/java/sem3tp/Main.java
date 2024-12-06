package sem3tp;

import sem3tp.Board.BoardBase;
import sem3tp.Builder.BoardBuilder;
import sem3tp.Creator.Creator;
import sem3tp.Poles.InitPole;

public class Main {
    public static void main(String[] args) {
        Creator creator = Creator.getInstance();
        BoardBuilder builder = creator.createBoardBuilder(5);
        BoardBase base = builder.build(5);
        System.out.println("udalo sie stworzyc baze boarda");
    }
}