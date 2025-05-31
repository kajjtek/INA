#include "../astarEssentials.h" // Include the corresponding header file
#include <iostream> // Required for std::cout

uint8_t astarEssentials::ManhattanLC(State& state){
    switch (type)
    {
    case Puzzle::eightPuzzle:
        return doManhattan(state, 3) + 2*doLC(state, 3);
    case Puzzle::fifteenPuzzle:
        return doManhattan(state, 4) + 2*doLC(state, 4);
    default:
        return 0;
    }
}

int astarEssentials::doLC(State& state, int n) {
    return LCRow(state, n) + LCCol(state, n);
}

int astarEssentials::LCRow(State& state, int n){
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

bool astarEssentials::isInGoalRow(int index, int value, int n){
    int goalRow = value-1/n;
    int curRow = index/n;
    return goalRow == curRow;
}

bool astarEssentials::isInGoalCol(int index, int value, int n){
    int goalCol = value-1%n;
    int curCol = index%n;
    return goalCol == curCol;
}

int astarEssentials::LCCol(State& state, int n){
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

int astarEssentials::doManhattan(State& state, int n){
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

astarEssentials::astarEssentials(int type){
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

bool astarEssentials::isSolvable(State state, int zeroindex){
    int row = 4 - zeroindex/4;
    int inv = inversions(state,zeroindex);

    return (row+inv) % 2 == 1;
}

std::vector<int> astarEssentials::generateActions(int zeroIndex){
    std::vector<int> list;
    if(zeroIndex <= 11) list.push_back(4);
    if(zeroIndex/4 > 0 ) list.push_back(-4);
    if(zeroIndex%4 != 0) list.push_back(-1);
    if(zeroIndex%4 !=3) list.push_back(1);
    return list;
}

int astarEssentials::inversions(State& state, int zeroindex){
    int count = 0;
    for(int i=0; i<16; i++){
        if(state.get(i)==0x0) continue;
        for(int j=i; j<16; j++){
            if(state.get(j)==0x0) continue;
            if(state.get(i)>state.get(j)) count++;
        }
    }
    return count;
}

int astarEssentials::getH(Heurestics htype, State state){
    switch (htype)
    {
    case Heurestics::ManhattanLC:
        return ManhattanLC(state);
    case Heurestics::Manhattan:
        return doManhattan(state, 4);
    default:
        return 0;
        break;
    }
}

int astarEssentials::getG(State state, uint8_t heur){
    return state.distance-heur;
}

void astarEssentials::reconstructPath(State& state, std::unordered_set<State> explored, State& initial){
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
    std::cout<<"Path:"<<count<<" Explored states"<<explored.size()<<std::endl;
}