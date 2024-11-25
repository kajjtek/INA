#include "Simulation.h"
#include "Results.h"
#include "CSVwriter.h"
#include <stdio.h>
#include <iostream>
#include <chrono>

int main(){
   auto start = std::chrono::high_resolution_clock::now();
    Simulation simulation;
    CSVwriter writer;
    for(int k=1000; k<= 100000; k+=1000){
        writer.setN(k);
        simulation.setN(k);
        for (int i = 0; i < 50; i++){
            simulation.reset();
            Results result = simulation.executeSimulation();    
            writer.setResults(result);
            writer.writeFile();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;
    return 0;
}