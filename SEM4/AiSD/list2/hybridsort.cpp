#include <stdio.h>
#include <iostream>
#include "arrayPrinter.h"

template <class T>
class hybridSort{
    public:
    int* cmps;
    int* swps;
    hybridSort(){
        cmps = new int(0);
        swps = new int(0);
    }
    void hybridsort(int firstindex, int lastindex, int k, T* array){
        if(lastindex-firstindex+1>k){
            int pivotindex = partition(firstindex, lastindex, array);

            hybridsort(firstindex, pivotindex-1, k, array);
            hybridsort(pivotindex+1, lastindex, k, array);
        }else{
            insertionsort(lastindex-firstindex+1, array);
        }
    }
    private:
    void swap(T* left, T* right){
        T temp = *left;
        *left = *right;
        *right = temp;
    }
    int partition(int firstindex, int lastindex, T* array){
        T pivot = array[firstindex];
        int i = firstindex - 1; 
        int j = lastindex+1;
        while(i<=j){
            while(array[i]<=pivot) {i++;};
            while(array[j]>=pivot) {j--;};
            if(i>j) continue;
            swap(&array[i],&array[j]);
            i++;    
            j++;
        }
        return getIndex(pivot,array, firstindex, lastindex);
    }
    void insertionsort(int size, T* array){
        int i = 1;
        while(i<size){
            T x = array[i]; 
            int j=i;
            while(j>0 && array[j-1]>x){
                array[j]=array[j-1];
                j--;
            }
            array[j]=x;
            i++;
        }
    }
    int getIndex(T value, T* array, int first, int last){
        for(int i=first; i<=last;i++){
            if(array[i]==value) return i;
        }
        return -1;
    }
};

int* load_array(int* size){
    std::cin>>*size;
    int* array = new int[*size];
    for(int i=0; i<*size;i++){
        std::cin>>array[i];
    }
    return array;
}

int* copy_array(int size, int* array){
    int* temp = new int[size];
    for(int i=0; i<size; i++){
        temp[i]=array[i];
    }
    return temp;
}

int main(){
    int* array;
    int size=0;
    array = load_array(&size);
    int* temp = copy_array(size,array);
    std::cout << "Podaj wartosc k:" <<std::endl;
    int k;
    std::cin>>k;
    hybridSort<int> sorter;
    arrayPrinter<int> printer;
    sorter.hybridsort(0,size-1,k,array);
    std::cout<<"BEFORE:" <<std::endl;
    printer.print(size, temp);
    std::cout<<"AFTER:" <<std::endl;
    printer.print(size,array);
    std::cout<<"STATS:" <<std::endl;
    printer.printStats(*sorter.cmps, *sorter.swps);
    delete[] array;
}


