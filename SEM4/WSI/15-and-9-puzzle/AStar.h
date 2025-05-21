#ifndef ASTAR_H
#define ASTAR_H

#include <stdint.h>
#include "./objects/State.h"
#include "./operators/astarEssentials.h"
#include "./operators/Swapper.h"
#include <boost/heap/fibonacci_heap.hpp>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "./objects/Heurestics.h"
#include "./objects/Puzzle.h"

class AStar {
    private:
    int typePuzzle;
    Swapper swapper;
    Heurestics heuristic;
    astarEssentials essentials;

    const uint64_t aim_board = 0x0FEDCBA987654321;

    using Heap = boost::heap::fibonacci_heap<State, boost::heap::compare<std::greater<>>>;
    using Handle = Heap::handle_type;

    public:
    AStar(Heurestics h, int type)
    :typePuzzle(type),
    heuristic(h),
    essentials(type),
    swapper()
    {}

    void find(uint64_t initBoard) {
        std::unordered_set<State> explored;
        std::unordered_map<State, Handle> frontier_map;
        std::unordered_map<State, uint8_t> heurestic_map;
        State aim(aim_board);
        State init(initBoard);
        init.findZeroIndex();
        uint8_t x = essentials.getH(heuristic, init);
        init.distance=x;
        init.h=x;
        Heap frontier;
        auto in = frontier.push(init);
        frontier_map[init] = in;
        if(!essentials.isSolvable(init, init.getZeroIndex())) {std::cout<<"NOT SOLVABLE"<<std::endl; return;}
        while(!frontier.empty()){
            State current = frontier.top(); //we return const& but then constructor copies it into state -fine cause we care about board the most
            frontier.pop();
            frontier_map.erase(current);
            if(current == aim) {std::cout<<"FOUND A SOLUTION"<<std::endl;return essentials.reconstructPath(current, explored, init);}
            int zeroIndex = current.getZeroIndex();
            explored.insert(current);
            std::vector<int> possibleActions = essentials.generateActions(zeroIndex);
            uint8_t g = essentials.getG(current);
            g++;
            for(int move: possibleActions){
                int newIndexZero = zeroIndex + move;
                State potentialState = swapper.swapInBoard(current, zeroIndex, newIndexZero);
                if(!explored.contains(potentialState)){ //if it is in explored then it means we have already reached it with a shorter distance
                    potentialState.setZeroIndex(newIndexZero);
                    potentialState.setMove(move);
                    if(!frontier_map.contains(potentialState)){
                        potentialState.h=essentials.getH(heuristic, current);
                        potentialState.distance=g+potentialState.h;
                        auto handle = frontier.push(potentialState);
                        frontier_map[potentialState] = handle;
                    }
                    else{
                        auto handle = frontier_map[potentialState];
                        potentialState.distance=g+(*handle).h;
                        if(potentialState.distance<(*handle).distance){
                            frontier.decrease(handle, potentialState);
                        }
                    }
                }
            }
        }
        std::cout<<"got to the end"<<std::endl;
    }
};

#endif