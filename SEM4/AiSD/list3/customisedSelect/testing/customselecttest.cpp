#include "../../standardHandler.h"
#include "../customisedSelect.h"
#include <iostream>

int main(int argc, char* argv[]){
    if(argc<2){
        std::cerr << "input a parameter!" <<std::endl;
        return 1;
    }
    int k = std::atoi(argv[1]);
    standardHandler<int> handler;
    int size=0;
    int* array = handler.load_array(&size);
    int* temp = handler.copy_array(size,array);
    customSelect<int> selector;
    arrayPrinter<int> printer;
    int kvalue = selector.select(array, 0, size-1, k, 3);
    std::cout << "DESIRED VALUE: " << kvalue <<std::endl;
    std::cout<<"BEFORE:" <<std::endl;
    printer.print(size, temp);
    std::cout<<"AFTER:" <<std::endl;
    printer.print(size,array);
    std::cout<<"STATS:" <<std::endl;
    printer.printStats(selector.cmps, selector.swps);
    delete[] array;
}