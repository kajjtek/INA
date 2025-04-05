#include <stdio.h>
#include <iostream>
#include "generator.h"

int main(){
    generator<int> gen;
    int choice = 1;
    // std::cin>>choice;
    int size = 15;
    // std::cin>>size;
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
    std::cout<<size;
    for(int i=0; i<size;i++){
        std::cout<<array[i];
    }

    delete[] array;
    return 0;
}