#include "../objects/State.h"
#include "./Swapper.h"

class Mover {
    public:
    void makeMove(State& state, int n){
        Swapper swapper;
        int indexzero = state.getZeroIndex();
        int indexgoal = indexzero;
        if(state.move==0x0) indexgoal-=n;
        else if(state.move==0x1) indexgoal++;
        else if(state.move==0x2) indexgoal+=n;
        else if(state.move==0x3) indexgoal--;
        swapper.swapInBoardVoid(state, indexzero, indexgoal);
    }
};