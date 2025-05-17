#include "../Select/Select.h"

template <class T>
class Quicksort {
     public:
     int cmps = 0;
     int swps = 0;
     void sort(T* arr, int left, int right) {
        if(left>=right) return;
        int size = right-left+1;
        int rank = ((size)/2)+1;
        Select<T> select;
        int p = select.select(arr, left, right, rank);
        this->cmps+=select.cmps;
        this->swps+=select.swps;
        int mid = left+rank-1;
        sort(arr, left, mid-1);
        sort(arr,mid+1, right);
     }
     bool isSorted(T* sorted, T* copy, int size){
        std::sort(copy, copy+size);
        for(int i=0; i<size; i++){
            if(sorted[i]!=copy[i]) return false;
        }
        return true;
    }
};