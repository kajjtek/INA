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
    uint8_t prepareMove(int move);

    public:
    std::uint8_t distance = 0;
    std::uint8_t moveAndIndex;
    std::uint64_t board;

    State(uint64_t board);

    int getZeroIndex();
    int getMove();
    void findZeroIndex();
    void setMove(int value);
    void setZeroIndex(uint8_t index);
    std::uint8_t get(int index);
    void set(int index, std::uint8_t value);

    bool operator==(const State& other) const;
    bool operator<(const State& other) const;
    bool operator>(const State& other) const;
};

namespace std {
    template<>
    struct hash<State>{
        size_t operator()(const State& state) const noexcept{
            return std::hash<uint64_t>()(state.board);
        }
    };
}

#endif // STATE_H