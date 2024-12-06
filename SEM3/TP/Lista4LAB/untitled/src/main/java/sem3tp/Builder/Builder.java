package sem3tp.Builder;

import sem3tp.Board.Board;
import sem3tp.Board.BoardBase;

public interface Builder {
    public BoardBase build(int layers_num);
}
