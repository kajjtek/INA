#include "../../../CSVwriter.h"
#include "../../../generator.h"
#include "../binSearch.h"
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <random>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

int getRandom(int left, int right){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(left,right);
    return distribution(gen);
}

int main(){
    generator<int> gen;
    CSVwriter writernormal("end_binsearch_test.csv");
    writernormal.header("n,cmps,time");
    for(int k=0; k<50; k++){
        for(int n = 1000; n<=100000; n+=1000){
            binSearch<int> searcher('m');
            int* array = gen.generateAscendingArray(n);
            auto t1 = high_resolution_clock::now();
            searcher.find(array, 0, n-1, (n)-10);
            auto t2 = high_resolution_clock::now();
            double time = duration_cast<nanoseconds>(t2 - t1).count();
            writernormal.writeFile(n, searcher.cmps, time);
        }
    }
    writernormal.~CSVwriter();
    CSVwriter writerrandom("end_binsearch_random_test.csv");
    writerrandom.header("n,cmps,time");
    for(int k=0; k<50; k++){
        for(int n = 1000; n<=100000; n+=1000){
            binSearch<int> searcher('r');
            int* array = gen.generateAscendingArray(n);
            auto t3 = high_resolution_clock::now();
            searcher.find(array, 0, n-1, (n)-10);
            auto t4 = high_resolution_clock::now();
            double time = duration_cast<nanoseconds>(t4 - t3).count();
            writerrandom.writeFile(n, searcher.cmps, time);
        }
    }
    writerrandom.~CSVwriter();
}