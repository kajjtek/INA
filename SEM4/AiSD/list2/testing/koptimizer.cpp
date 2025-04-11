#include "../../CSVwriter.h"
#include "../hybridsort.h"
#include "../../generator.h"
#include <chrono>

int main(){
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    generator<int> gen;
    CSVwriter writer("../csvfiles/koptimal.csv");
    int conf[] = {1000};
    for(int n:conf){
        for(int i=0; i<10;i++){
        int* original = gen.generateRandomArray(n);
        for(int k=0; k<100; k++){
            hybridSort<int> sorter;
            int* array = gen.copy_array(n, original);
            auto t1 = high_resolution_clock::now();
            sorter.hybridsort(0,n-1,k,array);
            auto t2 = high_resolution_clock::now();
            writer.writeFile(k,duration_cast<milliseconds>(t2 - t1).count(),*sorter.swps);
        }
    }
    }
    writer.~CSVwriter();
    return 0;
}