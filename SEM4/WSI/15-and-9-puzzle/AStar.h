#include <stdint.h>
#include "./objects/State.h"
#include "../operators/astarEssentials.h"
#include "../operators/Swapper.h"
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
    Heurestics h;
    astarEssentials essentials;

    const uint64_t aim = 0xFEDCBA9876543210;

    using Heap = boost::heap::fibonacci_heap<State, boost::heap::compare<std::greater<>>>;
    using Handle = Heap::handle_type;

    public:
    AStar(Heurestics h, int type)
    :typePuzzle(type),
    h(h),
    essentials(type),
    swapper()
    {}

    void find(uint64_t initBoard) {
        std::unordered_set<State> explored;
        std::unordered_map<State, Handle> frontier_map;
        State init(initBoard);
        Heap frontier;
        auto in = frontier.push(init);
        frontier_map[init] = in;
        while(!frontier.empty()){
            State current = frontier.top(); //we return const& but then constructor copies it into state -fine cause we care about board the most
            frontier.pop();
            frontier_map.erase(current);
            if(current == aim) return essentials.reconstructPath(current);
            int zeroIndex = current.getZeroIndex();
            if(!essentials.isSolvable(current, zeroIndex)) continue;
            explored.insert(current);
            std::vector<int> possibleActions = essentials.generateActions(zeroIndex);
            for(int move: possibleActions){
                int newIndexZero = zeroIndex + move;
                State potentialState = swapper.swapInBoard(current, zeroIndex, newIndexZero);
                if(!explored.contains(potentialState)){ //if it is in explored then it means we have already reached it with a shorter distance
                    uint8_t g = essentials.getG(this->h, current);
                    g++;
                    potentialState.distance = g + essentials.getH(h, potentialState);
                    potentialState.move=move;
                    if(!frontier_map.contains(potentialState)){
                         //tutaj h.value i heurestyka jaka typowa dla astar occurence.
                         //enum that calculates the heurestic
                        auto handle = frontier.push(potentialState);
                        frontier_map[potentialState] = handle;
                    }
                    else{
                        auto handle = frontier_map[potentialState];
                        if(potentialState.distance<(*handle).distance){
                            frontier.decrease(handle, potentialState);
                        }
                    }
                }
            }
        }
    }
};