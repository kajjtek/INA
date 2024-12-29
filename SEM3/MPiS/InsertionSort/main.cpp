#include "Sort.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include "CSVWriter.h"
using namespace std;

    std::random_device rd;           // Seed source
    std::mt19937 generator(rd());    // Mersenne Twister PRNG

vector<int> GenerateRandomPermutation(int n){
    vector<int> vector;
    for (int i = 1; i <= n; i++)
    {
        vector.push_back(i);
    }
    shuffle(vector.begin(), vector.end(), generator);//uniform distrribution guaranteed
    return vector;
    }

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    CSVwriter writer;

    for(int n=100; n<=10000; n+=100){
        for (int k = 0; k < 50; k++)
        {
            vector<int> vector = GenerateRandomPermutation(n);
            Sort sorter;
            sorter.setArrayInt(vector);
            sorter.InsertionSort();
            writer.setN(n);
            writer.setResults(sorter.getComparisons(),sorter.getSwaps());
            writer.writeFile();
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;
    return 0;
}

