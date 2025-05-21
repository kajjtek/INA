#ifndef MOVER_H
#define MOVER_H

#include "../objects/State.h"
#include "./Swapper.h"

class Mover {
    public:
    void makeInverseMove(State& state, int n){
        Swapper swapper;
        int indexzero = state.getZeroIndex();
        int indexgoal = indexzero;
        if(state.getMove()==0x0) indexgoal+=n;
        else if(state.getMove()==0x1) indexgoal--;
        else if(state.getMove()==0x2) indexgoal-=n;
        else if(state.getMove()==0x3) indexgoal++;
        swapper.swapInBoardVoid(state, indexzero, indexgoal);
    }
    void makeMove(State& state, int n){
        Swapper swapper;
        int indexzero = state.getZeroIndex();
        int indexgoal = indexzero;
        if(state.getMove()==0x0) indexgoal-=n;
        else if(state.getMove()==0x1) indexgoal++;
        else if(state.getMove()==0x2) indexgoal+=n;
        else if(state.getMove()==0x3) indexgoal--;
        swapper.swapInBoardVoid(state, indexzero, indexgoal);
    }
};

#endif