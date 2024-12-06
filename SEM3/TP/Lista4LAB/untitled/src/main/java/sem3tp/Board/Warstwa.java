//package sem3tp.Board;
//
//import sem3tp.Poles.Pole;
//import sem3tp.Poles.StandardPole;
//
//import java.util.ArrayList;
//import java.util.Iterator;
//
//public class Warstwa {
//    ArrayList<Pole> poleArrayList = new ArrayList<>();
//    Iterator<Pole> poleIterator;
//    int id;
//    Warstwa nextwarstwa;
//    int max;
//
//    public Warstwa(int id){
//        this.id=id;
//        this.max=6*(id-1);
//    }
//
//    public void generatePoles(){
//        for(int i=0;i<6*(id-1);i++){
//            poleArrayList.add(new StandardPole(i,id));
//        }
//    }
//
//    public void execute(){
//        poleIterator= poleArrayList.iterator();
//        nextwarstwa = new Warstwa(id+1);
//        nextwarstwa.generatePoles();
//        if(this.id<5){//zmienna do dodania
//            this.updateNeighours();
//            nextwarstwa.execute();
//        } else if (this.id==5) {
//            lastUpdate();
//        }
//    }
//
//    public void updateNeighours(){
//        int j=0;
//        Pole temp = null;
//        while(poleIterator.hasNext()){
//            StandardPole next = (StandardPole) poleIterator.next();
//            next.neighbours.add(poleArrayList.get((next.getId()+1)%max));
//            next.neighbours.add(poleArrayList.get((next.getId()-1+max)%max));
//            int next_size = next.neighbours.size();
//            for(int i=0; i<(Math.min(6-next_size,2));i++){
//                temp=nextwarstwa.poleArrayList.get(j);
//                next.addNeighbour(temp);
//                j++;
//            }
//            StandardPole rightNeighbour = (StandardPole) poleArrayList.get((next.getId()+1)%max);
//            if (temp!=null){rightNeighbour.addNeighbour(temp);}
//        }
//    }
//
//    public void lastUpdate(){
//        int j=0;
//        Pole temp = null;
//        while(poleIterator.hasNext()){
//            StandardPole next = (StandardPole) poleIterator.next();
//            next.neighbours.add(poleArrayList.get((next.getId()+1)%max));
//            next.neighbours.add(poleArrayList.get((next.getId()-1+max)%max));
//        }
//    }
//
//    public Pole getPole(int index){
//        return poleArrayList.get(index);
//    }
//
//}
