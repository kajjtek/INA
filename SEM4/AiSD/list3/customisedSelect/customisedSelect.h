#include <random>
#include "../../arrayPrinter.h"
#include <cmath>


template <class T>
class customSelect {
    public:
    arrayPrinter<int> printer;
    int cmps = 0;
    int swps = 0;
    int select(T* array, int left, int right, int aim, int div){
        cmps++;
        if(left==right) return array[left];
        int r = findPivot(array, left, right, div);
        int k = r - left + 1;
        cmps++;
        // printer.print(right-left+1,array);
        if(aim == k) return array[r];
        if(aim < k) {cmps++; return select(array, left, r-1, aim, div);}
        else {cmps++;return select(array, r+1, right, aim-k, div);}
    }
    private:
    int findPivot(T* array, int left, int right, int div){
        int size = right-left+1;
        int nGroups = std::ceil(size/(double) div);
        int* medians = new int[nGroups];
        int nLast = size - div*(nGroups-1);
        int i = 0;
        while(i<nGroups-1){
            insertionsort(left+div*i, left+div*(i+1)-1, array);
            medians[i] = array[left+div*i+2];
            i++;
            cmps++;
        }
        insertionsort(left+div*i, left+div*i+nLast-1, array);
        medians[i] = array[left+div*i+nLast/2];
        customSelect<int> select2;
        int median = select2.select(medians, 0, nGroups-1, (nGroups/2),div);
        int index = left;
        swps++;
        while(index<=right && array[index]!=median){
            index++;
            cmps++;
        }
        swap(&array[right],&array[index]);
        delete[] medians;

        return partition(left, right, array);
    }
    int partition(int firstindex, int lastindex, T* array){
        T pivot = array[lastindex];
        int i = firstindex - 1; 
        int j = firstindex;
        while(j<lastindex){
            cmps+=2;
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
            cmps++;
            T x = array[i]; 
            int j=i;
            while(j>left && array[j-1]>x){
                cmps++;
                array[j]=array[j-1];
                j--;
            }
            array[j]=x;
            i++;
        }
    }
};