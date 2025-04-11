#include <stdio.h>
#include <iostream>


template <class T>
class InsertionSort{
    public:
    int* cmps = new int(0);
    int* swps = new int(0);
    void sort(int size, T* array){
        int i = 1;
        while(i<size){
            T x = array[i];
            int j=i;
            while(j>0 && array[j-1]>x){
                array[j]=array[j-1];
                j--;
                (*this->cmps)++;
                (*this->swps)++;
            }
            array[j]=x;
            (*this->swps)++;
            i++;
        }
    }
    bool isSorted(T* sorted, T* copy, int size){
        std::sort(copy, copy+size);
        for(int i=0; i<size; i++){
            if(sorted[i]!=copy[i]) return false;
        }
        return true;
    }
};


