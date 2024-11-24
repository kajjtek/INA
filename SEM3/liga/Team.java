public class Team implements Comparable<Team> {
    String nazwa;
    int punkty;

    Team(int punkty,String nazwa){
        this.nazwa=nazwa;
        this.punkty=punkty;
    }

    public int getScore(){
        return punkty;
    }

    @Override
    public int compareTo(Team y){
        return Integer.compare(this.punkty,y.punkty);
    }
}
