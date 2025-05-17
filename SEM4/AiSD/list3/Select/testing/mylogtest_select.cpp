#include "../Select.h"
#include "../../../generator.h"
#include "../../../arrayPrinter.h"

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
        int* array = gen.generateRandomArray(n);
        int* temp = gen.copy_array(n ,array);
        int size = n;
        Select<int> select;
        int k = getRandom(1,n);
        int kvalue = select.select(array, 0, n-1, k);
        std::cout<<"-----------------------------"<<std::endl;
        std::cout<<"SELECT - RANDOM ARRAY" << std::endl;
        std::cout << "DESIRED INDEX: " <<k <<std::endl;
        std::cout << "DESIRED VALUE: " << kvalue <<std::endl;
        std::cout<<"BEFORE:" <<std::endl;
        printer.print(size, temp);
        std::cout<<"AFTER:" <<std::endl;
        printer.print(size,array);
        std::cout<<"STATS:" <<std::endl;
        printer.printStats(select.cmps, select.swps);
        std::cout<<"SORTED:" <<std::endl;
        std::sort(array, array+size);
        printer.print(size, array);
        delete[] array;
    }
    for(int n=16; n<=30; n++){
        int* array = gen.generateAscendingArray(n);
        int* temp = gen.copy_array(n ,array);
        int size = n;
        Select<int> select;
        int k = getRandom(1,n);
        int kvalue = select.select(array, 0, n-1, k);
        std::cout<<"-----------------------------"<<std::endl;
        std::cout<<"SELECT - ASCENDING ARRAY" << std::endl;
        std::cout << "DESIRED INDEX: " <<k <<std::endl;
        std::cout << "DESIRED VALUE: " << kvalue <<std::endl;
        std::cout<<"BEFORE:" <<std::endl;
        printer.print(size, temp);
        std::cout<<"AFTER:" <<std::endl;
        printer.print(size,array);
        std::cout<<"STATS:" <<std::endl;
        printer.printStats(select.cmps, select.swps);
        std::cout<<"SORTED:" <<std::endl;
        std::sort(array, array+size);
        printer.print(size, array);
        delete[] array;
    }
    for(int n=16; n<=30; n++){
        int* array = gen.generateDescendingArray(n);
        int* temp = gen.copy_array(n ,array);
        int size = n;
        Select<int> select;
        int k = getRandom(1,n);
        int kvalue = select.select(array, 0, n-1, k);
        std::cout<<"-----------------------------"<<std::endl;
        std::cout<<"SELECT - DESCENDING ARRAY" << std::endl;
        std::cout << "DESIRED INDEX: " <<k <<std::endl;
        std::cout << "DESIRED VALUE: " << kvalue <<std::endl;
        std::cout<<"BEFORE:" <<std::endl;
        printer.print(size, temp);
        std::cout<<"AFTER:" <<std::endl;
        printer.print(size,array);
        std::cout<<"STATS:" <<std::endl;
        printer.printStats(select.cmps, select.swps);
        std::cout<<"SORTED:" <<std::endl;
        std::sort(array, array+size);
        printer.print(size, array);
        delete[] array;
    }
}