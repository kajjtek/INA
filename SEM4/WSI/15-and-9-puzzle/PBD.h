#ifndef PBD_H
#define PBD_H

#include <stdint.h>
#include "./objects/State.h"

class PBD {
    public:
    void preparePBD(){
        uint64_t goal1 = 0x1234560000000000;
        uint64_t goal2 = 0x000000789ABC0000;
        uint64_t goal3 = 0x000000000000DEF0;
        execute(goal1);
        execute(goal2);
        execute(goal3);
    }
    private:
    void execute(uint64_t goal){
        State goalState(goal);
        
    }
};

#endif