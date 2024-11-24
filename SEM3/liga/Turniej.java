import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

public class Turniej {
    ArrayList<Mecz> spotkania = new ArrayList<>();
    int[] points = {0,1,2};
    Team cum = new Team(4, "CUM");
    Team wsb = new Team(3, "wsb");
    Team ken = new Team(3, "kenwin");
    Team solvro = new Team(0, "solvro");
    Team[] teams = {cum, ken,wsb,solvro};
    double c = 0;
    double przejscie=0;
    Team ekipa = cum;

    void zeroing(){
        cum.punkty=4;
        ken.punkty=4;
        wsb.punkty=2;
        solvro.punkty=0;
}
    
    public boolean isPlayed(String name1, String name2){
        return (name1.equals("CUM")&&name2.equals("kenwin")||
        (name1.equals("kenwin")&&name2.equals("solvro"))||
        (name1.equals("wsb")&&name2.equals("solvro"))||
        (name1.equals("CUM")&&name2.equals("solvro"))||
        (name1.equals("kenwin")&&name2.equals("wsb")));
    }
    void createMatches(){
        for(int i=0;i<4;i++){
            for(int j=i+1;j<4;j++){
                if(isPlayed(teams[i].nazwa, teams[j].nazwa)){
                    continue;
                }
                spotkania.add(new Mecz(teams[i], teams[j]));
            }
        }
    }

    void simulateMatch(int x, ArrayList<Integer> tab){
        if(x<spotkania.size()){
            x++;
            for(int i=0;i<3;i++){
                tab.add(points[i]);
                simulateMatch(x,tab);
                tab.remove(tab.size()-1);
            }
        }else{
            for(int z=0;z<spotkania.size();z++){
                Mecz mecz = spotkania.get(z);
                mecz.druzyna1.punkty+=tab.get(z);
                mecz.druzyna2.punkty+=2-tab.get(z);
            }
            Arrays.sort(teams,Collections.reverseOrder());
            if(teams[1].punkty==teams[2].punkty){
                przejscie++;
                if(ekipa.punkty>=teams[1].punkty){
                    c++;
                }
            }
            if((teams[0].nazwa.equals(ekipa.nazwa)||teams[1].nazwa.equals(ekipa.nazwa))&&!(teams[1].punkty==teams[2].punkty&&teams[0].punkty==teams[1].punkty)){
                c++;
            }
            przejscie++;
            zeroing();           
        }

    }

    void simulation(){
        ArrayList<Integer> tab = new ArrayList<>();
        simulateMatch(0, tab);
        System.out.println(c/przejscie);
    }

    
}
