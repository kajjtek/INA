#include "../../../CSVwriter.h"
#include "../DualpivotQuicksort.h"
#include "../../../generator.h"
#include "../../../arrayPrinter.h"
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

int main() {
    generator<int> gen;
    CSVwriter quickSortWriter("./list3/csvfiles/dualsort.csv");
    quickSortWriter.header("n,cmps,time");
    for(int i=0; i<50; i++){
        for(int n=100; n<=50000; n+=100){
            DualpivotQuicksort<int> selector;
            int* array = gen.generateRandomArray(n);
            auto t1 = high_resolution_clock::now();
            selector.sort(array,0,n-1);
            auto t2 = high_resolution_clock::now();
            double time = duration_cast<microseconds>(t2 - t1).count();
            quickSortWriter.writeFile(n,selector.cmps, time);
            delete[] array;
        }
    }
    quickSortWriter.~CSVwriter();
}