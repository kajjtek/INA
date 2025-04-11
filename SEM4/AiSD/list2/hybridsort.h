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
        if(lastindex-firstindex>k){
            int pivotindex = partition(firstindex, lastindex, array);

            hybridsort(firstindex, pivotindex, k, array);
            hybridsort(pivotindex+1, lastindex, k, array);
            arrayPrinter<int> printer;
            printer.print(lastindex-firstindex+1,array);
        }else if(lastindex-firstindex>0){
            insertionsort(firstindex, lastindex, array);
        }
    }
    bool isSorted(T* sorted, T* copy, int size){
        std::sort(copy, copy+size);
        for(int i=0; i<size; i++){
            if(sorted[i]!=copy[i]) return false;
        }
        return true;
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
            do
            {
                i++;
                (*this->cmps)++;
            } while (array[i]<pivot);
            do
            {
                j--;
                (*this->cmps)++;
            } while (array[j]>pivot);
            if(i>=j) return j;
            swap(&array[i],&array[j]);
            (*this->swps)++;
        }
        return getIndex(pivot,array, firstindex, lastindex);
    }
    void insertionsort(int left, int right, T* array){
        int i = left+1;
        while(i<=right){
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

