#include "../../CSVwriter.h"
#include "../mySort.h"
#include "../../generator.h"
#include "../MergeSort.h"


int main(){
    generator<int> gen;
    int repetitions[] = {1,10, 100};
    for(int k : repetitions){
        CSVwriter writer("../csvfiles/mergesort"+std::to_string(k)+".csv");
        for(int i=0; i<k; i++){
            for(int n=1000; n<=50000; n+=1000){
                MergeSort<int> sorter;
                int* array = gen.generateRandomArray(n);
                array = sorter.sort(array,0 ,n-1);
                writer.writeFile(n,*sorter.cmps, *sorter.swps);
                delete[] array;
            }
        }
        writer.~CSVwriter();
    }
}