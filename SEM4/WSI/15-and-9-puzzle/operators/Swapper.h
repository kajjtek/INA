#include <stdint.h>
#include "../objects/State.h"

class Swapper {
    public:
    State swapInBoard(State oldstate, int oldindex, int newindex){
        uint64_t board = oldstate.board;
        uint8_t v1 = oldstate.get(oldindex);
        uint8_t v2 = oldstate.get(newindex);
        State state(board);
        state.set(oldindex, v2);
        state.set(newindex, v1);
        return state;
    }
    void swapInBoardVoid(State& oldstate, int oldindex, int newindex){
        uint8_t v1 = oldstate.get(oldindex);
        uint8_t v2 = oldstate.get(newindex);
        oldstate.set(oldindex, v2);
        oldstate.set(newindex, v1);
    }
};