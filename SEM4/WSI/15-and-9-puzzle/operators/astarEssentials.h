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
    //PRIVATE//
    private:
    Puzzle type;
    uint8_t ManhattanLC(State& state){
        switch (type)
        {
        case Puzzle::eightPuzzle:
            return doManhattan(state, 3) + 2*doLC(state, 3);
        case Puzzle::fifteenPuzzle:
            return doManhattan(state, 4) + 2*doLC(state, 4);
        default:
            break;
        }
    }
    int doLC(State& state, int n) {
        // return 0;
        return LCRow(state, n) + LCCol(state, n);
    }
    int LCRow(State& state, int n){
        int count = 0;
        std::vector<u_int8_t> array;
        for(int i=0; i<n*n; i++){
            u_int8_t current = state.get(i);
            if(isInGoalRow(i, current, n) && current!=0x0){
                array.push_back(current);
            }
            if(i%n==n-1){
                BIT bit;
                count+=bit.getDefaultInversionsCount(array);
                array.clear();
            }
        }
        return count;
    }
    bool isInGoalRow(int index, int value, int n){
        int goalRow = value-1/n;
        int curRow = index/n;
        return goalRow == curRow;
    }
    bool isInGoalCol(int index, int value, int n){
        int goalCol = value-1%n;
        int curCol = index%n;
        return goalCol == curCol;
    }
    int LCCol(State& state, int n){
        int count = 0;
        std::vector<u_int8_t> array;
        for(int col = 0; col < n; ++col){ // Iterate through each column index
            for(int row = 0; row < n; ++row){ // Iterate through each row for the current column
                int index = row * n + col; // Calculate board index for (row, col)
                u_int8_t current = state.get(index);
                // Check if the tile 'current' at 'index' is in its goal column AND belongs to this current 'col'
                if(current != 0x0 && isInGoalCol(index, current, n)){ // isInGoalCol correctly checks if current tile's goal col matches 'col'
                    array.push_back(current);
                }
        }
        BIT bit; //
        count += bit.getDefaultInversionsCount(array); //
        array.clear();
    }
    return count;
    }
    int doManhattan(State& state, int n){
        int sum = 0;
        for(int i=0; i<n*n; i++){
            uint8_t current = state.get(i);
            if(current==0x0)continue;
            int goalindex = current-1;
            int cur_row = i/n;
            int cur_col = i%n;
            int goal_row = goalindex/n;
            int goal_col = goalindex%n;
            sum+=abs(cur_row-goal_row)+abs(cur_col-goal_col);
        }
        return sum;
    }
    // PUBLIC //
    public:
    astarEssentials(int type){
        switch (type)
        {
        case 8:
            this->type=Puzzle::eightPuzzle;
            break;
        case 15:
            this->type=Puzzle::fifteenPuzzle;
            break;
        default:
            break;
        }
    }
    bool isSolvable(State state, int zeroindex){
        int row = 4 - zeroindex/4;
        int inv = inversions(state,zeroindex);

        return (row+inv) % 2 == 1;
        // if(row%2==0 || inv%2==1) { //second-last for example and parity odd
        //     return true;
        // }
        // if(row%2==1 || inv%2==0){
        //     return true;
        // } //last and even parity
        // return false;
    }
    std::vector<int> generateActions(int zeroIndex){
        std::vector<int> list;
        if(zeroIndex <= 11) list.push_back(4);
        if(zeroIndex/4 > 0 ) list.push_back(-4);
        if(zeroIndex%4 != 0) list.push_back(-1);
        if(zeroIndex%4 !=3) list.push_back(1);
        return list;
    }
    int inversions(State& state, int zeroindex){
        int count = 0;
        for(int i=0; i<16; i++){
            if(state.get(i)==0x0) continue;
            for(int j=i; j<16; j++){
                if(state.get(j)==0x0) continue;
                if(state.get(i)>state.get(j)) count++;
            }
        }
        return count;
        // BIT bit;
        // return bit.getInversionsCount(state);
    }
    int getH(Heurestics htype, State state){
        switch (htype)
        {
        case Heurestics::ManhattanLC:
            return ManhattanLC(state);
            // break;
        // case Heurestics::PBD:
        //     return PBD(state);
        //     // break;
        default:
            return 0;
            break;
        }
    }
    int getG(State state){
        return state.distance-state.h;
    }
    
    // uint8_t PBD(State state){ //1. Preprocessing o data - the board is divided into 6-6-3
    //     State newState(state.board);
    //     const uint8_t starter = 0x0;
    //     State div1 = preprocessPBD(state, 0, 6);
    //     State div2 = preprocessPBD(state, 6, 6);
    //     State div3 = preprocessPBD(state, 12, 2);

    // }

    // State preprocessPBD(State state, int startindex,int n){
    //     State temp(state.board);
    //     while(temp.size()<n){
    //         int val = state.get(startindex);
    //         if (val==0){startindex++; continue;}

    //     }
    //     return temp;
    // }
    void reconstructPath(State& state, std::unordered_set<State> explored, State& initial){
        std::cout<<"FOUND THE SOLUTION"<<std::endl;
        int count=0;
        Mover mover;
        StatePrinter printer;
        int n;
        switch (type)
        {
        case Puzzle::eightPuzzle:
            n = 3;
            break;

        case Puzzle::fifteenPuzzle:
            n = 4;
            break;
        default:
            break;
        }
        printer.doPrint(state);
        while(state!=initial){
            mover.makeInverseMove(state,n);
            printer.doPrint(state);
            auto it = explored.find(state);
            if(it!=explored.end()) state = *it;
            count++;
        }
        std::cout<<"Path:"<<count<<std::endl;
    }
};

#endif