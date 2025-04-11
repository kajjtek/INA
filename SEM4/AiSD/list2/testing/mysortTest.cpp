#include "../mySort.h"
#include "../../arrayPrinter.h"

int* copy_array(int size, int* array){
    int* temp = new int[size];
    for(int i=0; i<size; i++){
        temp[i]=array[i];
    }
    return temp;
}

int* load_array(int* size){
    std::cin.read(reinterpret_cast<char*>(size),sizeof(size));
    int* array = new int[*size];
    for(int i=0; i<*size;i++){
        std::cin.read(reinterpret_cast<char*>(&array[i]),sizeof(array[i]));
    }
    return array;
}


int main(){
    int size=0;
    int* array = load_array(&size);
    int* temp = copy_array(size,array);
    mySort<int> sorter;
    arrayPrinter<int> printer;
    array = sorter.sort(array,size);
    std::cout<<"BEFORE:" <<std::endl;
    printer.print(size, temp);
    std::cout<<"AFTER:" <<std::endl;
    printer.print(size,array);
    std::cout<<"STATS:" <<std::endl;
    printer.printStats(*sorter.cmps, *sorter.swps);
    if(sorter.isSorted(array,temp,size)) std::cout<<"Prawidlowo posortowane" <<std::endl;
    else std::cout<<"Nieprawidlowo posortowane" <<std::endl;
    delete[] array;
}
