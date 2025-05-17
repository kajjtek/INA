#include "../../CSVwriter.h"
#include "../Select/Select.h"
#include "../RandomSelect/RandomSelect.h"
#include "../../generator.h"
#include "../../arrayPrinter.h"


int main(){
    generator<int> gen;
    int ks[] = {16,32,50,75};
    for(int k : ks){
    CSVwriter insertionWriter("./list3/csvfiles/select"+std::to_string(k)+".csv");
    insertionWriter.header("n,cmps,swps");
    for(int i=0; i<50; i++){
        for(int n=100; n<=50000; n+=100){
            RandomSelect<int> selector;
            int* array = gen.generateRandomArray(n);
            selector.select(array,0,n-1,k);
            insertionWriter.writeFile(n,selector.cmps, selector.swps);
            delete[] array;
        }
    }
    insertionWriter.~CSVwriter();
    CSVwriter quickSortWriter("./list3/csvfiles/randomselect"+std::to_string(k)+".csv");
    quickSortWriter.header("n,cmps,swps");
    for(int i=0; i<50; i++){
        for(int n=100; n<=50000; n+=100){
            Select<int> selector;
            int* array = gen.generateRandomArray(n);
            selector.select(array,0,n-1,k);
            quickSortWriter.writeFile(n,selector.cmps, selector.swps);
            delete[] array;
        }
    }
    quickSortWriter.~CSVwriter();
}
}