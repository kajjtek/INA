#ifndef BIT_H
#define BIT_H

#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include "./State.h"
#include <vector>

class BIT {
    public:
    int getDefaultInversionsCount(std::vector<u_int8_t> array);
    int getInversionsCount(State& state);
    private:
    int getSum(int tree[], int current);
    void update(int tree[], int n, int index, int value);
};
#endif