#include "../mySort.h"
#include "../../arrayPrinter.h"
#include "../../generator.h"

int main(){
    generator<int> gen;
    arrayPrinter<int> printer;
    for(int n=8; n<=32; n++){
        int* iarray = gen.generateRandomArray(n);
        mySort<int> isort;
        isort.sort(iarray, n);
        std::cout<<"My SORT"<<std::endl;
        printer.printStats(*isort.cmps, *isort.swps);
    }
    for(int n=8; n<=32; n++){
        int* iarray = gen.generateAscendingArray(n);
        mySort<int> isort;
        isort.sort(iarray, n);
        std::cout<<"MY SORT"<<std::endl;
        printer.printStats(*isort.cmps, *isort.swps);
    }
    for(int n=8; n<=32; n++){
        int* iarray = gen.generateDescendingArray(n);
        mySort<int> isort;
        isort.sort(iarray, n);
        std::cout<<"MY SORT"<<std::endl;
        printer.printStats(*isort.cmps, *isort.swps);
        
    }
}