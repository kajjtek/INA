#include <random>
#include <iostream>
#include "CSVwriter.h"

class Simulation{
    private:
    void getResults(int* container, int size, int n);
    int getRandomVariable();
    int getSn(int N);
    void initialiseArray(int* container, int size);
    double getPn(int N);
    public:
    void execute();
    void executeExercise3();
};

void Simulation::initialiseArray(int* container, int size){
    for (int i = 0; i < size; i++)
    {
        container[i]=0;
    }
}

void Simulation::execute(){
    std::cout<<"Zadanie  2 - symulacja"<<std::endl;
    std::cout<<"Podaj N"<<std::endl;
    int N;
    std::cin>>N;
    int size = 2*N+1;
    int tab[size];

    initialiseArray(tab,size);
    getResults(tab, size, N);

    CSVwriter writer;
    writer.setN(N);
    writer.setArray(tab);

    for (int i = 0; i < size; i++)
    {
        // writer.writeFile(i);
    }
    
    std::cout<<"----------------"<<std::endl;
}

void Simulation::getResults(int* container, int size, int n){
    for(int i=0;i<100000;i++){
        int S = getSn(n);
        container[S+n]++;
    }
}

int Simulation::getSn(int N){
    int suma=0;
    for(int i=0;i<N;i++){
        suma+=getRandomVariable();
    }
    return suma;
}

int Simulation::getRandomVariable(){
    std::random_device rd;
    std::mt19937 generator(rd());
    std::bernoulli_distribution distribution(0.5);

    if(distribution(generator))return 1;
    return -1;
}

double Simulation::getPn(int N){
    double count = 0;
    int suma = 0;
    for (int i = 0; i < N; i++)
    {
       int previoussuma = suma;
       suma+=getRandomVariable();
       if(suma>0 || previoussuma>0){
        count++;
       } 
    }
    double time = (double)N;
    double Pn = count/time;
    return Pn;
}

void Simulation::executeExercise3(){
    CSVwriter writer;
    std::cout<<"Zadanie 3 - Symulacja"<<std::endl;
    
    int N;
    std::cin>>N;

    for(int k=1; k<=5000; k++){
        double Pn = getPn(N);
        writer.writeFile(k,Pn);
    }

        std::cout<<"----------------"<<std::endl;
}