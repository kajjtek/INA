#ifndef ASTAR_H
#define ASTAR_H

#include <stdint.h>
#include <vector>
#include <unordered_set>
#include <unordered_map>
// #include <boost/heap/fibonacci_heap.hpp>
#include <functional> 
#include <queue>

#include "./objects/State.h"
#include "./operators/astarEssentials.h"
#include "./operators/Swapper.h"
#include "./objects/Heurestics.h"
#include "./objects/Puzzle.h"

class AStar {
    private:
    int typePuzzle;
    Swapper swapper;
    Heurestics heuristic;
    astarEssentials essentials;

    const uint64_t aim_board = 0x0FEDCBA987654321;

    using Heap = std::priority_queue<State, std::vector<State>, std::greater<State>>;

    // using Heap = boost::heap::fibonacci_heap<State, boost::heap::compare<std::greater<State>>>;
    // using Handle = Heap::handle_type;

    public:
    AStar(Heurestics h, int type);

    void find(uint64_t initBoard); 
};

#endif