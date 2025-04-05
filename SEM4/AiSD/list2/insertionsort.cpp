#include <stdio.h>

template <class T>
class InsertionSort{
    public:
    void sort(int size, T* array){
        int i = 1;
        while(i<n){
            T x = array[i] 
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

