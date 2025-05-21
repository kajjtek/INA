#ifndef STATE_H
#define STATE_H

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
    private:
    uint8_t prepareMove(int move){
        if(move==4) return 0x2;
        if(move==-1) return 0x3;
        if(move==-4) return 0x0;
        return 0x1;
    }
    public:
    std::uint8_t distance = 0;
    std::uint8_t h = 0;
    std::uint8_t moveAndIndex;
    std::uint64_t board;
    State(uint64_t board) {
        this->board = board;
    }
    int getZeroIndex(){
        return moveAndIndex >> 2;
    }
    int getMove(){
        return 0b00000011 & moveAndIndex;
    }
    void findZeroIndex(){
        for(int i=0; i<16; i++){
            if(get(i)==0x0) setZeroIndex(i);
        }
    }
    void setMove(int value){
        uint8_t prepared = prepareMove(value);
        uint8_t zero = getZeroIndex();
        this->moveAndIndex = zero << 2 | 0x3 & prepared;
    }
    void setZeroIndex(uint8_t index){
        uint8_t move = getMove();
        this->moveAndIndex = index << 2 | 0x3 & move;
    }
    std::uint8_t get(int index){
        return (board >> (index*4)) & 0xF;
    }
    void set(int index, std::uint8_t value){
        std::uint64_t shift_amount = static_cast<std::uint64_t>(index) * 4;

    // Mask to clear the specific 4-bit nibble
    std::uint64_t mask_for_clearing = static_cast<std::uint64_t>(0xF) << shift_amount;

    // Value to be placed, shifted to the correct position
    std::uint64_t value_to_place = static_cast<std::uint64_t>(value) << shift_amount;

    this->board = (this->board & ~mask_for_clearing) | value_to_place;
    }
    bool operator==(const State& other) const{
        return this->board == other.board;
    }
    bool operator<(const State& other) const{
        return this->distance<other.distance;
    }
    bool operator>(const State& other) const {
        return other < *this;
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

#endif