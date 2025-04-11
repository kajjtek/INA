#include <random>
#include "../../arrayPrinter.h"
#include <cmath>


template <class T>
class Select {
    public:
    arrayPrinter<int> printer;
    int cmps = 0;
    int swps = 0;
    int select(T* array, int left, int right, int aim){
        cmps++;
        if(left==right) return array[left];
        int r = findPivot(array, left, right);
        int k = r - left + 1;
        cmps++;
        printer.print(right-left+1,array);
        if(aim == k) return array[r];
        if(aim < k) {cmps++; return select(array, left, r-1, aim);}
        else {cmps++;return select(array, r+1, right, aim-k);}
    }
    private:
    int findPivot(T* array, int left, int right){
        int size = right-left+1;
        int nGroups = std::ceil(size/5);
        int* medians = new int[nGroups];
        int nLast = size - 5*(nGroups-1);
        int i;
        for(i=0; i<nGroups-1; i++){
            insertionsort(left+5*i, left+5*(i+1)-1, array);
            medians[i] = array[left+ 5*i+2];
        }
        i++;
        insertionsort(left+ 5*i, left+5*i+nLast-1, array);
        medians[i] = array[nLast/2];
        int medianIndex = select(medians, 0, nGroups-1, (nGroups/2));
        swap(&array[right],&array[medianIndex]);

        return partition(left, right, array);
    }
    int partition(int firstindex, int lastindex, T* array){
        T pivot = array[lastindex];
        int i = firstindex - 1; 
        int j = firstindex;
        while(j<lastindex){
            if(array[j]<=pivot){
                swap(&array[++i],&array[j]);
                swps++;
            }
            j++;
        }
        swap(&array[++i],&array[lastindex]);
        swps++;
        return i;
    }
    void swap(T* left, T* right){
        T temp = *left;
        *left = *right;
        *right = temp;
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
};