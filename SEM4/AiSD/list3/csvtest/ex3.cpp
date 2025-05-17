#include "../../CSVwriter.h"
#include "../customisedSelect/customisedSelect.h"
#include "../RandomSelect/RandomSelect.h"
#include "../../generator.h"
#include <chrono>

int main(){
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    generator<int> gen;
    int divs[] = {3,5,7,9};
    int ks[] = {32,75};
    for(int div : divs){
    for(int k : ks){
    CSVwriter quickSortWriter("./list3/csvfiles/customSelect"+std::to_string(div)+"_"+std::to_string(k)+".csv");
    quickSortWriter.header("n,cmps,swps,time");
    for(int i=0; i<50; i++){
        for(int n=100; n<=50000; n+=100){
            customSelect<int> selector;
            int* array = gen.generateRandomArray(n);
            auto t1 = high_resolution_clock::now();
            selector.select(array,0,n-1,k, div);
            auto t2 = high_resolution_clock::now();
            double time = duration_cast<milliseconds>(t2 - t1).count();
            quickSortWriter.writeFileWithTime(n,selector.cmps, selector.swps, time);
            delete[] array;
        }
    }
    quickSortWriter.~CSVwriter();
}
}
}