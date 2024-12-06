package sem3tp.Mover;

import sem3tp.Creator.Creator;
import sem3tp.Poles.Pole;
import sem3tp.Poles.StandardPole;

public class Mover {
    int layers;
    Creator create = Creator.getInstance();

    private static Mover instance;

    public static Mover getInstance(int x) {
        if(instance==null){
            synchronized (Creator.class) {
                if(instance==null){
                    instance= new Mover(x);
                }
            }
        }
        return instance;
    }

    private Mover(int layers){
        this.layers=layers;
    }

    private boolean isWithinBoundary(int x, int y, int z){
        return Math.abs(x)<layers && Math.abs(y)<layers && Math.abs(z)<layers;
    }

    public Pole move(Pole pole, Directions direction){
        int newX = pole.getxCord()+ direction.addXCord();
        int newY = pole.getyCord() + direction.addYCord();
        int newZ = pole.getzCord()+ direction.addZCord();
        if(isWithinBoundary(newX,newY,newZ)) {return create.createSTDPole(newX, newY, newZ);}
        return null;
    }
}
