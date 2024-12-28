#include "Simulation.h"
#include "Results.h"
#include "CSVwriter.h"
#include <stdio.h>
#include <iostream>
#include <chrono>

int main(){
   auto start = std::chrono::high_resolution_clock::now();
    Simulation simulation;
    std:: cout << "State which simulation u want to perfrom - 0. zadanie2, 1. zadanie3"<<std::endl;
    int type;
    std:: cin >> type;
    CSVwriter writer(type);
    for(int k=1000; k<= 100000; k+=1000){
        writer.setN(k);
        simulation.setN(k);
        for (int i = 0; i < 50; i++){
            simulation.reset();
            if(type==0){
                Results result = simulation.executeSimulation();
            }
            Results result = simulation.executeSimulationEX3();    
            writer.setResults(result);
            if(type==0) writer.writeFile();
            else writer.writeFileEX3();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;
    return 0;
}