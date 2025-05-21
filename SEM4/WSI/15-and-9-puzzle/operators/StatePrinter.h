#ifndef STATEPRINTER_H
#define STATEPRINTER_H

#include "../objects/State.h"
#include <iostream>

class StatePrinter {
    public:
    void doPrint(State state){
        std::cout<<"-----STATE-----"<<std::endl;
        for(int i=0; i<16; i++){
            std::cout<< static_cast<int>(state.get(i))<<+" ";
            if(i%4==3) std::cout<<std::endl;
        }
        std::cout<<"---------------"<<std::endl;
    }
};

#endif