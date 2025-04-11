#include "../../CSVwriter.h"
#include "../insertionsort.h"
#include "../../generator.h"
#include "../hybridsort.h"

int main(){
    generator<int> gen;
    int repetitions[] = {1, 10, 100};
    for(int k : repetitions){
        CSVwriter insertionWriter("../csvfiles/insertion"+std::to_string(k)+".csv");
        for(int i=0; i<k; i++){
            for(int n=10; n<=50; n+=10){
                InsertionSort<int> inSorter;
                int* array = gen.generateRandomArray(n);
                inSorter.sort(n,array);
                insertionWriter.writeFile(n,*inSorter.cmps, *inSorter.swps);
            }
        }
        insertionWriter.~CSVwriter();
        CSVwriter quickSortWriter("../csvfiles/quicksort"+std::to_string(k)+".csv");
        for(int i=0; i<k; i++){
            for(int n=10; n<=50; n+=10){
                hybridSort<int> sorter;
                int* array = gen.generateRandomArray(n);
                sorter.hybridsort(0, n-1,0 ,array);
                quickSortWriter.writeFile(n,*sorter.cmps, *sorter.swps);
            }
        }
        quickSortWriter.~CSVwriter();
        CSVwriter hybridSortWriter("../csvfiles/hybridsort"+std::to_string(k)+".csv");
        for(int i=0; i<k; i++){
            for(int n=10; n<=50; n+=10){
                hybridSort<int> sorter;
                int* array = gen.generateRandomArray(n);
                sorter.hybridsort(0, n-1,32 ,array);
                hybridSortWriter.writeFile(n,*sorter.cmps, *sorter.swps);
            }
        }
    }
}