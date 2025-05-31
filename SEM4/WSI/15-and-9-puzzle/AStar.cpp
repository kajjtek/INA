#include "AStar.h" 
#include <iostream>  

AStar::AStar(Heurestics h, int type)
    :typePuzzle(type),
    heuristic(h),
    essentials(type),
    swapper()
    {}

void AStar::find(uint64_t initBoard) {
    std::unordered_set<State> explored;
    // std::unordered_map<State, Handle> frontier_map;
    std::unordered_map<State, uint8_t> heurestics_map;
    State aim(aim_board);
    State init(initBoard);
    init.findZeroIndex();
    uint8_t x = essentials.getH(heuristic, init);
    init.distance=x;
    // init.h=x;
    Heap frontier;
    frontier.push(init);
    heurestics_map[init]=x;
    // frontier_map[init] = in;
    if(!essentials.isSolvable(init, init.getZeroIndex())) {std::cout<<"NOT SOLVABLE"<<std::endl; return;}
    while(!frontier.empty()){
        State current = frontier.top(); //we return const& but then constructor copies it into state -fine cause we care about board the most
        frontier.pop();
        uint8_t heurestic_value = heurestics_map[current];
        heurestics_map.erase(current);

        if(explored.contains(current)) continue;

        // frontier_map.erase(current);
        if(current == aim) {std::cout<<"FOUND A SOLUTION"<<std::endl;return essentials.reconstructPath(current, explored, init);}
        int zeroIndex = current.getZeroIndex();
        explored.insert(current);
        std::vector<int> possibleActions = essentials.generateActions(zeroIndex);
        uint8_t g = essentials.getG(current, heurestic_value);
        g++;
        for(int move: possibleActions){
            int newIndexZero = zeroIndex + move;
            State potentialState = swapper.swapInBoard(current, zeroIndex, newIndexZero);
            if(!explored.contains(potentialState)){ //if it is in explored then it means we have already reached it with a shorter distance
                potentialState.setZeroIndex(newIndexZero);
                potentialState.setMove(move);
                uint8_t h2;
                if(heurestics_map.contains(potentialState)) h2=heurestics_map[potentialState];
                else {h2=essentials.getH(heuristic, potentialState);heurestics_map[potentialState]=h2;}
                potentialState.distance=g+h2;
                frontier.push(potentialState);
            }
        }
    }
    std::cout<<"got to the end"<<std::endl;
}