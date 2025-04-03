#include <stdio.h>
#include <random>

#ifndef GENERALTESTER_H
#define GENERALTESTER_H

class GeneralTester {
    public:
    int* randomArrayGenerator(int size){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0,100000);
        int *array = new int[size];
        for(int i=0; i<size;i++){
            array[i]=dist(gen);
        }
        return array;
    }

    int randomNumber(int size){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0,size-1);
        return dist(gen);
    }

    int randomBin(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0,1);
        return dist(gen);
    }
};

#endif