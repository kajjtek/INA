#include "../insertionsort.h"
#include "../../generator.h"
#include "../hybridsort.h"
#include "../dualpivot.h"
#include "../../arrayPrinter.h"

int main(){
    generator<int> gen;
    arrayPrinter<int> printer;
    for(int n=8; n<=32; n++){
        int* iarray = gen.generateRandomArray(n);
        int* darray = gen.copy_array(n, iarray);
        int* qarray = gen.copy_array(n, iarray);
        int* harray = gen.copy_array(n, iarray);
        InsertionSort<int> isort;
        isort.sort(n,iarray);
        std::cout<<"INSERTION SORT"<<std::endl;
        printer.printStats(*isort.cmps, *isort.swps);
        hybridSort<int> hsort;
        hsort.hybridsort(0,n-1,16,harray);
        std::cout<<"HYBRID SORT"<<std::endl;
        printer.printStats(*hsort.cmps, *hsort.swps);
        hybridSort<int> qsort;
        qsort.hybridsort(0,n-1,0,qarray);
        std::cout<<"QUICKSORT"<<std::endl;
        printer.printStats(*qsort.cmps, *qsort.swps);
        DualPivot<int> dsort;
        dsort.sort(0,n-1,darray);
        std::cout<<"DUALPIVOT SORT"<<std::endl;
        printer.printStats(*dsort.cmps, *dsort.swps);
    }
    for(int n=8; n<=32; n++){
        int* iarray = gen.generateAscendingArray(n);
        int* darray = gen.copy_array(n, iarray);
        int* qarray = gen.copy_array(n, iarray);
        int* harray = gen.copy_array(n, iarray);
        InsertionSort<int> isort;
        isort.sort(n,iarray);
        std::cout<<"INSERTION SORT"<<std::endl;
        printer.printStats(*isort.cmps, *isort.swps);
        hybridSort<int> hsort;
        hsort.hybridsort(0,n-1,16,harray);
        std::cout<<"HYBRID SORT"<<std::endl;
        printer.printStats(*hsort.cmps, *hsort.swps);
        hybridSort<int> qsort;
        qsort.hybridsort(0,n-1,0,qarray);
        std::cout<<"QUICKSORT"<<std::endl;
        printer.printStats(*qsort.cmps, *qsort.swps);
        DualPivot<int> dsort;
        dsort.sort(0,n-1,darray);
        std::cout<<"DUALPIVOT SORT"<<std::endl;
        printer.printStats(*dsort.cmps, *dsort.swps);
    }
    for(int n=8; n<=32; n++){
        int* iarray = gen.generateDescendingArray(n);
        int* darray = gen.copy_array(n, iarray);
        int* qarray = gen.copy_array(n, iarray);
        int* harray = gen.copy_array(n, iarray);
        InsertionSort<int> isort;
        isort.sort(n,iarray);
        std::cout<<"INSERTION SORT"<<std::endl;
        printer.printStats(*isort.cmps, *isort.swps);
        hybridSort<int> hsort;
        hsort.hybridsort(0,n-1,16,harray);
        std::cout<<"HYBRID SORT"<<std::endl;
        printer.printStats(*hsort.cmps, *hsort.swps);
        hybridSort<int> qsort;
        qsort.hybridsort(0,n-1,0,qarray);
        std::cout<<"QUICKSORT"<<std::endl;
        printer.printStats(*qsort.cmps, *qsort.swps);
        DualPivot<int> dsort;
        dsort.sort(0,n-1,darray);
        std::cout<<"DUALPIVOT SORT"<<std::endl;
        printer.printStats(*dsort.cmps, *dsort.swps);
    }
}