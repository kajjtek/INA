#include <cstdint>
#include <functional>

// A state that uses only 80 bits of memory that allows for memory efficient search.
// For move such convention is to be followed:
// 0 - NORTH
// 1 - EAST
// 2 - SOUTH
// 3 - WEST

//index is ordered from right for the board
// when comparing uisng std:: greater we have new>old if true old stays on the top else moves in the list
//thus when we swap to std::less we have new<old if true old stays ...
class State {
    public:
    std::uint8_t distance = 0;
    std::uint8_t move;
    std::uint64_t board;
    State(uint64_t board) {
        this->board = board;
    }
    std::uint8_t get(int index){
        return (board >> (index*4)) & 0xF;
    }
    int getZeroIndex(){
        for(int i=0; i<16;i++){
            if(get(i)==0x0) return i;
        }
        return -1;
    }
    void set(int index, std::uint8_t value){
        std::uint64_t mask = 0xF << (index*4);
        this->board = (board & ~mask) | (value << (index*4));
    }
    bool operator==(const State& other){
        return this->board == other.board;
    }
    bool operator<(const State& other){
        return this->distance<other.distance;
    }
};

namespace std {
    template<>
    struct hash<State>{
        size_t operator()(const State& state) const noexcept{
            return std::hash<uint64_t>()(state.board);
        }
    };
}