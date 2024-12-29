#include <random>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include "Origin.h"
#include "CSVwriter.h"
#include <string>
using namespace std;


int main(){
    string probability;
    auto start = std::chrono::high_resolution_clock::now();

    cout << "Input probability" <<endl;
    cin >> probability;

    CSVwriter writer;
    Origin origin;
    origin.setProbability(stod(probability));

    for (int n = 100; n <= 10000; n+=100)
    {
        writer.setN(n);
        origin.createChildren(n);
        for (int k = 0; k < 50; k++)
        {
            writer.setResults(origin.execute());
            writer.writeFile();
            origin.resetCounter();
            origin.resetChildren();
        }
    }
        
    

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;
    return 0;
}