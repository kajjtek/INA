package pt.university.puzzle8;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class Test {

    public static void main(String[] args){
        //     Random random = new Random();
        //     for(int n=1;n<=24;n++){
        //     HashSet<Node> explored = new HashSet<Node>();
        //     for(int i=0;i<10;i++){
        //         String[] first = {"0","1","2","3","4","5","6","7","8"};
        //         AStar aStar = new AStar(first);
        //         aStar.initialNode.state.setActions(aStar.ListOfAllPossibleActions);
        //         for(double j=1;j<1.3*n;j++){
        //             int x = random.nextInt(aStar.initialNode.state.actions.size());
        //             aStar.initialNode.swap(aStar.initialNode.getDesiredIndex(0), aStar.initialNode.state.actions.get(x)+aStar.initialNode.getDesiredIndex(0), aStar.ListOfAllPossibleActions);
        //         }
        //         if(!explored.contains(aStar.initialNode)){
        //             System.out.println(aStar.Search());
        //             explored.add(aStar.initialNode);
        //         };
        //     }
        //     System.out.println("");
        // }
        // AStar a = new AStar(args);
        // System.out.println(a.Search());
        File file = null;
        switch(Integer.parseInt(args[0])){
            case 2: file = new File("C:\\SEM3\\8PUZZLE\\cases\\2depth.txt");break;
            case 4:file = new File("C:\\SEM3\\8PUZZLE\\cases\\4depth.txt");break;
            case 6:file = new File("C:\\SEM3\\8PUZZLE\\cases\\6depth.txt");break;
            case 8:file = new File("C:\\SEM3\\8PUZZLE\\cases\\8depth.txt");break;
            case 10:file = new File("C:\\SEM3\\8PUZZLE\\cases\\10depth.txt");break;
            case 12:file = new File("C:\\SEM3\\8PUZZLE\\cases\\12depth.txt");break;
            case 14:file = new File("C:\\SEM3\\8PUZZLE\\cases\\14depth.txt");break;
            case 16:file = new File("C:\\SEM3\\8PUZZLE\\cases\\16depth.txt");break;
            case 18:file = new File("C:\\SEM3\\8PUZZLE\\cases\\18depth.txt");break;
            case 20:file = new File("C:\\SEM3\\8PUZZLE\\cases\\20depth.txt");break;
            default:System.out.println("Liczba wieksza od 20 badz nie bedaca wielokrotnascia 2");System.exit(0);
        }
        if(file!=null){
            try{
                BufferedReader br = new BufferedReader(new FileReader(file));
                String s;
                int suma = 0;
                while((s=br.readLine())!=null){
                    char[] chararray = s.toCharArray();
                    String[] stringarray = new String[chararray.length];

                    for(int i=0;i<chararray.length;i++){
                        stringarray[i]=String.valueOf(chararray[i]);
                    }

                    AStar a = new AStar(stringarray);
                    int n = a.Search();
                    suma += n;
                }
                br.close();
                System.out.println(suma);} catch (IOException e) {
                e.printStackTrace();
            }

        }
    }
}
