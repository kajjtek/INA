package sem3tp.Poles;

import sem3tp.Storage.PoleStorage;

import java.util.ArrayList;

public abstract class Pole implements Comparable<Pole>{
    private int xCord, yCord, zCord;
//    private int warstwa;
//    private int max;
    PoleStorage neighbours = new PoleStorage();

    public void setzCord(int zCord) {
        this.zCord = zCord;
    }

    public int getzCord() {
        return zCord;
    }

    public void setxCord(int xCord) {
        this.xCord = xCord;
    }
    public void setyCord(int yCord) {
        this.yCord = yCord;
    }

    public int getxCord() {
        return xCord;
    }

    public int getyCord() {
        return yCord;
    }

    public PoleStorage getNeighbours() {
        return neighbours;
    }

    public void addNeighbour(Pole pole){
        if(!neighbours.contains(pole))this.neighbours.insert(pole);
//        pole.neighbours.add(this);
    }

    @Override
    public int compareTo(Pole pole2) {
        return Double.compare(Math.sqrt(Math.pow(this.getxCord(),2)+Math.pow(this.getyCord(),2))+Math.pow(this.getzCord(),2), Math.sqrt(Math.pow(pole2.getxCord(),2)+Math.pow(pole2.getyCord(),2)+Math.pow(pole2.getyCord(),2)));
    }

    @Override
    public boolean equals(Object obj) {
        if(obj instanceof Pole temp){
            return temp.getxCord() == this.xCord && temp.getyCord() == this.yCord && temp.getzCord()==this.zCord;
        }
        return false;
    }

    //
//    public void setWarstwa(int warstwa) {
//        this.warstwa = warstwa;
//        this.max=6*(warstwa-1);
//    }

//    public int getId() {
//        return id;
//    }

//    public int getWarstwa() {
//        return warstwa;
//    }
}
