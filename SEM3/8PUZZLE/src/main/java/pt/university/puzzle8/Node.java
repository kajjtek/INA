package pt.university.puzzle8;
import java.util.ArrayList;

public class Node implements Comparable<Node>{
    public State state;
    public int cost;
    public int expectedcost;

    Node(ArrayList<ArrayList<Integer>> list){
        this.state = new State(list);
    }

    public int heurystyka1(){
        int heurystyka1 = 0;
        for(int i=0;i<state.Board.length;i++){
            if(state.Board[i]!=0 && state.Board[i]!=i){
                heurystyka1++;
            }
        }
        return heurystyka1;
    }

    public int MeasureDistanceFromAim(int wartosc, int docelowyindex){
        int wiersz1 = wartosc/3;
        int wiersz2 = docelowyindex/3;
        int roznicawierszy = Math.abs(wiersz1-wiersz2);
        int roznicakolumn = Math.abs((wartosc%3) - (docelowyindex%3));
        return roznicakolumn+roznicawierszy;
    }

    public int getDesiredIndex(int n){
        for(int i = 0; i<9;i++){
            if(this.state.Board[i]==n){return i;}
        }
        return 100;
    }

    public int heurystyka2(){
        int sumaodleglosci = 0;
        for(int i = 0; i<state.Board.length;i++){
            sumaodleglosci += MeasureDistanceFromAim(this.state.Board[i],getDesiredIndex(this.state.Board[i]));
        }
        return sumaodleglosci;
    }

    public int parity(int[] board){
        int inwersje = 0;
        for(int i=0;i<board.length;i++){
            for(int j=i+1;j<board.length;j++){
                if(board[i]>board[j]){
                    inwersje++;
                }
            }
            if(board[i]==0 && i%2==1){
                inwersje++;
            }
        }
        return inwersje;
    }

    public boolean isSolvable(){
        int n = parity(this.state.Board);
        if(n%2==0){
            return true;
        }
        return false;
    }

    // public void swap(int zeroindex, int zeroindex2){
    //     int temp = this.state.Board[zeroindex];
    //     this.state.Board[zeroindex] = this.state.Board[zeroindex2];
    //     this.state.Board[zeroindex2] = temp;
    // }

    public void swap(int zeroindex, int zeroindex2, ArrayList<ArrayList<Integer>> list){
        int temp = this.state.Board[zeroindex];
        this.state.Board[zeroindex] = this.state.Board[zeroindex2];
        this.state.Board[zeroindex2] = temp;
        this.state.setActions(list);
    }

    public void copy(Node ToBeCopied){
        this.state.Copy(ToBeCopied.state);
        this.cost = ToBeCopied.cost;
    }

    @Override
    public int compareTo(Node node2){
        return Integer.compare(this.cost+this.expectedcost, node2.expectedcost+node2.cost);
    }

    @Override
    public boolean equals(Object obj){
        if(obj instanceof Node){return((Node)obj).state.equals(state);}
        else{return false;}
    }

    @Override
    public int hashCode(){
        return state.hashCode();
    }
}
