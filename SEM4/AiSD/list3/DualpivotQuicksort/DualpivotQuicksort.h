#include "../Select/Select.h"

template <class T>
class DualpivotQuicksort {
    public:
     int cmps = 0;
     int swps = 0;
     void sort(T* arr, int left, int right) {
        if(left>=right) return;
        int size = right-left+1;
        int rank1 = ((size)/3)+1;
        int rank2 = 2*rank1;
        Select<T> select;
        select.select(arr, left, right, rank1);
        int p1 = left+rank1 -1;

        int size2 = right - p1;
        rank2 -= rank1;
        select.select(arr, p1+1, right, rank2);
        int p2 = p1 + rank2 - 1;

        this->cmps+=select.cmps;
        this->swps+=select.swps;

        sort(arr, left, p1-1);
        sort(arr, p1+1, p2-1);
        sort(arr,p2+1, right);
     }
};