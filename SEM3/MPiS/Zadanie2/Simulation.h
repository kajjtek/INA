#ifndef SIMULATION_H
#define SIMULATION_H
#include "Results.h"
#include <vector>

class Simulation {
    private:
        int index_m = 1;
        int n;
        int oneball = 0;
        int twoball = 0;
        std::vector<int> boxes; 
        int flagone = 0;
        int flagtwo = 0;
        int recorder = 0;
        int maxIndex = 0;

    public:

        void verifyMax(int index);

        void reset();

        void populateBoxes();

        void incrementIndex_M();

        void setN(int n);
        
        void incrementOneBall();

        void incrementTwoball();

        void incrementFirstFlag();

        void incrementSecondFlag();

        void incrementRecorder();

        int MinimumInsertion(int d, std::mt19937& gen, std::uniform_int_distribution<int>& dis); //inserts the ball to the box with the lowest number of balls inside out of d randomly chosen boxes returns the index where it was inserted

        Results executeSimulation();

        Results executeSimulationEX3();

    
};


#endif