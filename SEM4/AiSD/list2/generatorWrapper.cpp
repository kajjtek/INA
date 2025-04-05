#include <stdio.h>
#include <iostream>
#include "generator.h"

int main(){
    generator<int> gen;
    int choice;
    std::cin>>choice;
    int size;
    std::cin>>size;
    int* array = nullptr;
    if(choice==1){
        array = gen.generateRandomArray(size);
    }
    else if(choice==2){
        array=gen.generateAscendingArray(size);
    }
    else if(choice==3){
        array=gen.generateDescendingArray(size);
    }
    else{
        std::cerr<<"error"<<std::endl;
    }
    sendArray(array,size);

    delete[] array;
    return 0;
}

void sendArray(int* arr, int size){
    std::cout.write(reinterpret_cast<const char*>(&size),sizeof(size));
    for(int i=0; i<size;i++){
        std::cout.write(reinterpret_cast<const char*>(&arr[i]),sizeof(arr[i]));
    }
}