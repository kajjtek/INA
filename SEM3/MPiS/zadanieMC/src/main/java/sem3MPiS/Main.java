package sem3MPiS;

import com.opencsv.CSVWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;
import java.util.Scanner;
import java.math.*;

public class Main{
    public static void main(String[] args){
        Scanner scanner = new Scanner(System.in);
        System.out.println("Enter a");
        double a = Double.parseDouble(scanner.nextLine());
        System.out.println("Enter b");
        String line = scanner.nextLine();
        double b;
        if(line.equals("pi")){
            b = Math.PI;
        }else {
            b = Double.parseDouble(line);
        }
        System.out.println("Wybierz funkcje 1, 2, 3");
        int number = Integer.parseInt(scanner.nextLine());
        System.out.println("podaj k - ilosc powtorzen");
        int k = Integer.parseInt(scanner.nextLine());
        scanner.close();
        double average;
        double suma=0;
        if(number==1){
            File file1 = new File("/Users/kajtek/Documents/STUDIA/WINTER24_25/MPiS_Zad1/Funkcja1.csv");
            try {
                new FileWriter(file1, false).close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            for(int n=50;n<=5000;n+=50){
                suma =0;
                for(int i=0;i<k;i++){
                    suma+=SyspodpunktA(a,b,n,file1);
                }
                average = suma /k;
                System.out.println("Dla funkcji "+number+" i powtorzen "+k+" oraz punktow" + n+" srednia wynosi = "+average);
            }
        }
        if(number==2){
            File file2 = new File("/Users/kajtek/Documents/STUDIA/WINTER24_25/MPiS_Zad1/Funkcja2.csv");
            try {
                new FileWriter(file2, false).close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            for(int n=50;n<=5000;n+=50){
                suma =0;
                for(int i=0;i<k;i++){
                    suma+=SyspodpunktB(a,b,n, file2);
                }
                average = suma /k;
                System.out.println("Dla funkcji "+number+" i powtorzen "+k+" oraz punktow" + n+" srednia wynosi = "+average);
            }
        }
        if(number==3){
            File file3 = new File("/Users/kajtek/Documents/STUDIA/WINTER24_25/MPiS_Zad1/Funkcja3.csv");
            try {
                new FileWriter(file3, false).close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            for(int n=50;n<=5000;n+=50){
                suma =0;
                for(int i=0;i<k;i++){
                    suma+=SyspodpunktC(a,b,n,file3);
                }
                average = suma /k;
                System.out.println("Dla funkcji "+number+" i powtorzen "+k+" oraz punktow" + n+" srednia wynosi = "+average);
            }
        }
    }

    static void lineWriterCSV(double Area,File file, int n){
        try {
            FileWriter outputfile = new FileWriter(file,true);

            CSVWriter writer = new CSVWriter(outputfile);

            writer.writeNext(new String[]{String.valueOf(Area),String.valueOf(n)});
            writer.close();
        }catch (IOException e){
            e.printStackTrace();
        }
    }

    static double randomNumberInRange(double a, double b){
        Random generator = new Random();
        double random = generator.nextDouble();
        return(a+(b-a)*random);
    }

    static double SyspodpunktA(double a,double b, int numberOfPoints, File file){
        double M = pierwszafunkcja(b);
        double C =0;
        for(int i=0;i<numberOfPoints;i++){
            double y = randomNumberInRange(0, M);
            double x = randomNumberInRange(a, b);
            if(y<=pierwszafunkcja(x)){
                C++;
            }
        }
        double probability = C/numberOfPoints;
        lineWriterCSV(probability*(b-a)*M,file,numberOfPoints);
        return probability*(b-a)*M;
    }

    static double SyspodpunktB(double a,double b, int numberOfPoints, File file){
        double M = 1;
        double C =0;
        for(int i=0;i<numberOfPoints;i++){
            double y = randomNumberInRange(0, M);
            double x = randomNumberInRange(a, b);
            if(y<=drugafunkcja(x)){
                C++;
            }
        }
        double probability = C/numberOfPoints;
        lineWriterCSV(probability*(b-a)*M,file,numberOfPoints);
        return probability*(b-a)*M;
    }

    static double SyspodpunktC(double a,double b, int numberOfPoints, File file){
        double M = trzeciafunkcja(0.25);
        double C =0;
        for(int i=0;i<numberOfPoints;i++){
            double y = randomNumberInRange(0, M);
            double x = randomNumberInRange(a, b);
            if(y<=trzeciafunkcja(x)){
                C++;
            }
        }
        double probability = C/numberOfPoints;
        lineWriterCSV(probability*(b-a)*M,file,numberOfPoints);
        return probability*(b-a)*M;
    }

    static double pierwszafunkcja(double x){
        return x*x*x;
    }

    static double drugafunkcja(double x){
        return Math.sin(x);
    }

    static double trzeciafunkcja(double x){
        return 4*x*(1-x)*(1-x)*(1-x);
    }
}

