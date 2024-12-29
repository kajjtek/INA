package sem3tp.Builder;

import sem3tp.Board.Board;
import sem3tp.Board.BoardBase;
import sem3tp.Creator.Creator;
import sem3tp.Mover.Directions;
import sem3tp.Mover.Mover;
import sem3tp.Poles.InitPole;
import sem3tp.Poles.Pole;
import sem3tp.Poles.StandardPole;
import sem3tp.Storage.PoleStorage;

public class BoardBuilder implements Builder{
    Creator creator;
    Mover mover;
    int maxPoles, layers_num;
    Directions[] possibleMoves = {Directions.East,Directions.West,Directions.NorthEast, Directions.NorthWest, Directions.SouthEast, Directions.SouthWest};

    public BoardBuilder(int layers){
    creator=Creator.getInstance();
    mover=Mover.getInstance(layers);
    }

    public void findMaxPoles(int layers_num){
        this.layers_num=layers_num;
        int suma=1;
        for(int i=2;i<=layers_num;i++){
            suma+=6*(i-1);
        }
        this.maxPoles=suma;
    }

    @Override
    public BoardBase build() {
        BoardBase boardBase = new BoardBase();
        PoleStorage poleList = new PoleStorage();
        findMaxPoles(layers_num);
        Pole current;
        poleList.insert(creator.createInitPole());
        for(int i=0;i< maxPoles;i++){
            current=poleList.getByIndex(i);
            addNeighbours(current, poleList);//kwestia iteratora potencjalnego
        }
        boardBase.setStorage(poleList);
        return boardBase;
    }

    public void addNeighbours(Pole currentPole, PoleStorage poleStorage){
        for (Directions direction : possibleMoves) {
            Pole temp = mover.move(currentPole, direction);
            if(temp==null){
                continue;}
            if (poleStorage.contains(temp)) {
                Pole existingPole = poleStorage.get(temp);
                currentPole.addNeighbour(existingPole);
            } else {
                poleStorage.insert(temp);
                currentPole.addNeighbour(temp);
            }
        }
    }
}
