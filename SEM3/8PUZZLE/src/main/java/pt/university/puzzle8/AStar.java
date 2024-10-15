package pt.university.puzzle8;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.PriorityQueue;

public class AStar {
    public Node initialNode;
    public HashSet<Node> explored = new HashSet<Node>();
    public PriorityQueue<Node> frontier = new PriorityQueue<Node>();
    public ArrayList<ArrayList<Integer>> ListOfAllPossibleActions = new ArrayList<>();

    public void findActions(){
        for(int index =0;index<=8;index++){
            ArrayList<Integer> PossibleActions = new ArrayList<Integer>();
            if(index%3!=0){
                PossibleActions.add(-1);
            }
            if(index%3!=2){
                PossibleActions.add(1);
            }
            if(index>=3){
                PossibleActions.add(-3);
            }
            if(index<=5){
                PossibleActions.add(3);
            }
            ListOfAllPossibleActions.add(PossibleActions);
        }
    }

    AStar(String[] args){
        findActions();
        initialNode = new Node(ListOfAllPossibleActions);
        for(int i =0; i<args.length;i++){
            initialNode.state.Board[i]=Integer.parseInt(args[i]);
        }
        initialNode.cost = 0;
        initialNode.state.setActions(this.ListOfAllPossibleActions);
    }

    int Search(){
        if(!initialNode.isSolvable()){System.out.println("Problem nierozwiazywalny");return 0;}
        Node aim = new Node(ListOfAllPossibleActions);
        int[] temp = {0,1,2,3,4,5,6,7,8};
        aim.state.Board = temp.clone();
        frontier.add(initialNode);
        Node current = new Node(ListOfAllPossibleActions);
        while(!frontier.isEmpty()){
            current.copy(frontier.poll());
            if(current.equals(aim)){return explored.size();}
            explored.add(current);
            for(int i = 0;i<current.state.actions.size();i++){
                Node NewNode = new Node(ListOfAllPossibleActions);
                NewNode.copy(current);
                int zeroindex = current.getDesiredIndex(0);
                int zeroindex2 = zeroindex + current.state.actions.get(i);
                NewNode.swap(zeroindex, zeroindex2,ListOfAllPossibleActions);
                if(!explored.contains(NewNode)){
                    if(!frontier.contains(NewNode)){
                        NewNode.cost = current.cost +1;
                        frontier.add(NewNode);
                    }
                    else{
                        if(NewNode.cost>=current.cost){NewNode.cost = current.cost;}//heurystyki dodac w pozniejszym czasie
                    }
                }
            }

        }
        return 0;
    }
}

