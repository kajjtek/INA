#include "./AStar.h"
#include "./objects/Heurestics.h"
#include "./operators/Generator.h"
#include "./operators/StatePrinter.h"
#include "./objects/BIT.h"

int main(){
    AStar aStar(Heurestics::ManhattanLC, 15);
    Generator generator;
    State state = generator.generateRandomState();
    StatePrinter printer;
    printer.doPrint(state);
    std::cout<<"RANDOM ARRAY"<<std::endl;
    aStar.find(state.board);
}