#include "../../CSVwriter.h"
#include "../insertionsort.h"
#include "../../generator.h"
#include "../hybridsort.h"

int main(){
    generator<int> gen;
    int repetitions[] = {100};
    for(int k : repetitions){
        CSVwriter quickSortWriter("../csvfiles/quicksort2_"+std::to_string(k)+".csv");
        for(int i=0; i<k; i++){
            for(int n=1000; n<=50000; n+=1000){
                hybridSort<int> sorter;
                int* array = gen.generateRandomArray(n);
                sorter.hybridsort(0, n-1,0 ,array);
                quickSortWriter.writeFile(n,*sorter.cmps, *sorter.swps);
            }
        }
        quickSortWriter.~CSVwriter();
        CSVwriter hybridSortWriter("../csvfiles/hybridsort2_"+std::to_string(k)+".csv");
        for(int i=0; i<k; i++){
            for(int n=1000; n<=50000; n+=1000){
                hybridSort<int> sorter;
                int* array = gen.generateRandomArray(n);
                sorter.hybridsort(0, n-1,0 ,array);
                hybridSortWriter.writeFile(n,*sorter.cmps, *sorter.swps);
            }
        }
    }
}