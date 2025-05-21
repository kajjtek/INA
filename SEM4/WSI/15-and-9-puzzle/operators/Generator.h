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
    State generateState(int n){
        astarEssentials essentials(4);
        Mover mover;
        std::random_device rd;
        std::mt19937 g(rd());
        State init(0x0FEDCBA987654321);
        init.findZeroIndex();
        for(int i=0; i<n; i++){
            std::vector<int> moves = essentials.generateActions(init.getZeroIndex());
            std::uniform_int_distribution<int> dist(0,moves.size()-1);
            init.setMove(moves.at(dist(g)));
            mover.makeMove(init, 4);
        }
        return init;
    }
};

#endif