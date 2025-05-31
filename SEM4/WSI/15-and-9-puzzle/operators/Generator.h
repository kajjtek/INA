#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdint.h>
#include <random>
#include <vector>
#include "./Mover.h"
#include "../objects/State.h"
#include "./astarEssentials.h"

class Generator {
    public:
    State generateRandomState(){
        std::vector<int> array = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        std::random_device rd;
        std::mt19937 g(rd());
        
        std::shuffle(array.begin(), array.end(), g);

        uint64_t board = 0x0;
        State state(board);
        int i=0;
        while(array.size()>0){
            int value = array.back();
            array.pop_back();
            state.set(i,value);
            i++;
        }
        return state;
    }
    int getOppositeMove(int lastMove) {
        if (lastMove == 0) return 2;
        if (lastMove == 1) return 3;
        if (lastMove == 2) return 0;
        if (lastMove == 3) return 1;
        return -1;
    }
    
    State generateState(int n) {
        astarEssentials essentials(4);
        Mover mover;
        std::random_device rd;
        std::mt19937 g(rd());
        State init(0x0FEDCBA987654321);
        init.findZeroIndex();
    
        
        int lastAppliedMove = -1; 
    
        for (int i = 0; i < n; ++i) {
            std::vector<int> possibleMoves = essentials.generateActions(init.getZeroIndex());
    
            
            int forbiddenMove = -1;
            if (lastAppliedMove != -1) { 
                forbiddenMove = getOppositeMove(lastAppliedMove);
            }
    
            
            
            possibleMoves.erase(std::remove_if(possibleMoves.begin(), possibleMoves.end(),
                                               [forbiddenMove](int move) {
                                                   return move == forbiddenMove;
                                               }),
                                possibleMoves.end());

            std::uniform_int_distribution<int> dist(0, possibleMoves.size() - 1);
            int chosenMove = possibleMoves.at(dist(g));
    
            
            init.setMove(chosenMove);
            mover.makeMove(init, 4);
    
            
            lastAppliedMove = chosenMove;
        }
        return init;
    }
};

#endif