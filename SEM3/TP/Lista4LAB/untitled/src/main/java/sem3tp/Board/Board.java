package sem3tp.Board;

import sem3tp.Poles.Pole;
import sem3tp.Storage.PoleStorage;

import java.util.ArrayList;

public abstract class Board implements BoardInterface {
    PoleStorage poleList;

    public void setStorage(PoleStorage poleList){
        this.poleList=poleList;
    }



}
