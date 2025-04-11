#include <random>
#include "../../arrayPrinter.h"

template <class T>
class RandomSelect {
    public:
    arrayPrinter<int> printer;
    int cmps = 0;
    int swps = 0;
    int select(T* array, int left, int right, int aim){
        cmps++;
        if(left==right) return array[left];
        int r = random_partition(array, left, right);
        int k = r - left + 1;
        cmps++;
        printer.print(right-left+1,array);
        if(aim == k) return array[r];
        if(aim < k) {cmps++ ;return select(array, left, r-1, aim);}
        else {cmps++;return select(array, r+1, right, aim-k);}
    }
    private:
    int getPivot(int left, int right){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution(left,right);
        return distribution(gen);
    }
    int random_partition(T* array, int left, int right){
        int pivot = getPivot(left, right);
        swap(&array[right], &array[pivot]);
        swps++;

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
};