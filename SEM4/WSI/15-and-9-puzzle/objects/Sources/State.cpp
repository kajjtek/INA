#include "../State.h"


State::State(uint64_t board) {
    this->board = board;
}

uint8_t State::prepareMove(int move){
    if(move==4) return 0x2;
    if(move==-1) return 0x3;
    if(move==-4) return 0x0;
    return 0x1;
}

int State::getZeroIndex(){
    return moveAndIndex >> 2;
}

int State::getMove(){
    return 0b00000011 & moveAndIndex;
}

void State::findZeroIndex(){
    for(int i=0; i<16; i++){
        if(get(i)==0x0) setZeroIndex(i);
    }
}

void State::setMove(int value){
    uint8_t prepared = prepareMove(value);
    uint8_t zero = getZeroIndex();
    this->moveAndIndex = zero << 2 | (0x3 & prepared);
}

void State::setZeroIndex(uint8_t index){
    uint8_t move = getMove();
    this->moveAndIndex = index << 2 | (0x3 & move);
}

std::uint8_t State::get(int index){
    return (board >> (index*4)) & 0xF;
}

void State::set(int index, std::uint8_t value){
    std::uint64_t shift_amount = static_cast<std::uint64_t>(index) * 4;

    // Mask to clear the specific 4-bit nibble
    std::uint64_t mask_for_clearing = static_cast<std::uint64_t>(0xF) << shift_amount;

    // Value to be placed, shifted to the correct position
    std::uint64_t value_to_place = static_cast<std::uint64_t>(value) << shift_amount;

    this->board = (this->board & ~mask_for_clearing) | value_to_place;
}

// Operator overloads definitions
bool State::operator==(const State& other) const{
    return this->board == other.board;
}

bool State::operator<(const State& other) const{
    return this->distance < other.distance;
}

bool State::operator>(const State& other) const {
    return other < *this;
}