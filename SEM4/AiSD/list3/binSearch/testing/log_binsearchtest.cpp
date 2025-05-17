#include "../binSearch.h"
#include <iostream>
#include <stdio.h>
#include "../../../generator.h"
#include "../../../arrayPrinter.h"
#include <random>

int getRandom(int left, int right){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(left,right);
    return distribution(gen);
}

int main(){
    arrayPrinter<int> printer;
    generator<int> gen;
    for(int n=16; n<=30; n++){
        std::cout<<"-----------------------------"<<std::endl;
        int* iarray = gen.generateAscendingArray(n);
        binSearch<int> selector('m');
        int result = selector.find(iarray, 0, n-1, getRandom(0,n+5));
        std::cout<<"Bin Search - descending arrray"<<std::endl;
        std::cout<<"RESULT: "<<result<<std::endl;
        printer.printStats(selector.cmps, 0);
        delete[] iarray;
    }
}