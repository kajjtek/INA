package pt.university.puzzle8;
import java.util.ArrayList;
import java.util.Arrays;


public class State {
    public int[] Board = new int[9];
    public ArrayList<Integer> actions;

    State() {
        actions = new ArrayList<>();
    }

    State(ArrayList<ArrayList<Integer>> list){
        this.setActions(list);
    }


    public void Copy(State ToBeCopied){
        this.Board = ToBeCopied.Board.clone();
        this.actions = new ArrayList<>(ToBeCopied.actions);
    }

    public int getDesiredIndex(int n){
        for(int i = 0; i<9;i++){
            if(this.Board[i]==n){return i;}
        }
        return 100;
    }

    public void setActions(ArrayList<ArrayList<Integer>> list){
        int i = getDesiredIndex(0);
        this.actions = new ArrayList<Integer>(list.get(i));
    }

    @Override
    public boolean equals(Object obj){
        if(obj instanceof State){return(Arrays.equals(((State)obj).Board,this.Board));}
        else{return false;}
    }

    @Override
    public int hashCode(){
        return Arrays.hashCode(Board);
    }
}
