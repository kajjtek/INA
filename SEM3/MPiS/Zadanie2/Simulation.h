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

    public:

        void reset();

        void populateBoxes();

        void incrementIndex_M();

        void setN(int n);
        
        void incrementOneBall();

        void incrementTwoball();

        void incrementFirstFlag();

        void incrementSecondFlag();

        void incrementRecorder();

        Results executeSimulation();
    
};


#endif