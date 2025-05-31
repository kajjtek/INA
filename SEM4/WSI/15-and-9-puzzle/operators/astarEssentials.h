#ifndef ASTARESSENTIALS_H
#define ASTARESSENTIALS_H

#include "../objects/State.h"
#include <vector>
#include "../objects/Heurestics.h"
#include "../objects/BIT.h"
#include "../objects/Puzzle.h"
#include <cmath>
#include "./StatePrinter.h"
#include "./Mover.h"
#include <unordered_set>
#include <stdint.h>


class astarEssentials {
    
    private:
    Puzzle type;
    uint8_t ManhattanLC(State& state);
    int doLC(State& state, int n);
    int LCRow(State& state, int n);
    bool isInGoalRow(int index, int value, int n);
    bool isInGoalCol(int index, int value, int n);
    int LCCol(State& state, int n);
    int doManhattan(State& state, int n);
    
    public:
    astarEssentials(int type);
    bool isSolvable(State state, int zeroindex);
    std::vector<int> generateActions(int zeroIndex);
    int inversions(State& state, int zeroindex);
    int getH(Heurestics htype, State state);
    int getG(State state, uint8_t heur);
    
    void reconstructPath(State& state, std::unordered_set<State> explored, State& initial);
};

#endif