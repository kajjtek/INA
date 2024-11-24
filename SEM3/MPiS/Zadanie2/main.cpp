#include "Simulation.h"
#include "Results.h"
#include <stdio.h>
#include <iostream>

int main(){
    int n,m;
    // std::cout << "Wpisz liczbe piłeczek" <<std::endl;
    // std::cin >> m;
    // std::cout << "Wpisz liczbe skrzynek" << std::endl;
    // std::cin >> n;
    Simulation simulation;
    simulation.setM(10);
    simulation.setN(3);
    simulation.populateBoxes();
    simulation.executeSimulation();

    return 0;
}