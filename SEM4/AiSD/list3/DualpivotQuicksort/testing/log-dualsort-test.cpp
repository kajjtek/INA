#include "../DualpivotQuicksort.h"
#include "../../../generator.h"
#include "../../../arrayPrinter.h"
#include <stdio.h>

int main(){
    generator<int> gen;
    arrayPrinter<int> printer;
    for(int n=8; n<=32; n++){
        std::cout<<"---------------------"<<std::endl;
        std::cout<<"DUAL SORT"<<std::endl;
        std::cout<<"BEFORE:"<<std::endl;
        int* iarray = gen.generateRandomArray(n);
        printer.print(n,iarray);
        int* copy = gen.copy_array(n,iarray);
        DualpivotQuicksort<int> isort;
        isort.sort(iarray, 0, n-1);
        std::cout<<"AFTER:"<<std::endl;
        printer.print(n, iarray);
        printer.printStats(isort.cmps, isort.swps);
        std::cout<<"VALIDATION"<<std::endl;
        std::sort(copy, copy+n);
        printer.print(n,copy);
    }
}