#include "../RandomSelect.h"
#include "../../../generator.h"

int getRandom(int left, int right){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(left,right);
    return distribution(gen);
}

int main(){
    generator<int> gen;
    arrayPrinter<int> printer;
    for(int n=16; n<=30; n++){
        std::cout<<"-----------------------------"<<std::endl;
        int* iarray = gen.generateRandomArray(n);
        RandomSelect<int> selector;
        selector.select(iarray, 0, n-1, getRandom(1,n));
        std::cout<<"RANDOM SElECT - random array"<<std::endl;
        printer.printStats(selector.cmps, selector.swps);
        delete[] iarray;
    }
    for(int n=16; n<=30; n++){
        std::cout<<"-----------------------------"<<std::endl;
        int* iarray = gen.generateAscendingArray(n);
        RandomSelect<int> selector;
        selector.select(iarray, 0, n-1, getRandom(1,n));
        std::cout<<"RANDOM SElECt - ascending array"<<std::endl;
        printer.printStats(selector.cmps, selector.swps);
        delete[] iarray;
    }
    for(int n=16; n<=30; n++){
        std::cout<<"-----------------------------"<<std::endl;
        int* iarray = gen.generateDescendingArray(n);
        RandomSelect<int> selector;
        selector.select(iarray, 0, n-1, getRandom(1,n));
        std::cout<<"RANDOM SElECt - descending arrray"<<std::endl;
        printer.printStats(selector.cmps, selector.swps);
        delete[] iarray;
    }
}