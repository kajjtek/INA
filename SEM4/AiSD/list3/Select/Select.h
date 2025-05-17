#include <random>
#include <cmath>

template <class T>
class Select {
    public:
    // arrayPrinter<int> printer;
    int cmps = 0;
    int swps = 0;
    int select(T* array, int left, int right, int aim){
        cmps++;
        if(left==right) return array[left];
        int r = findPivot(array, left, right);
        int k = r - left + 1;
        cmps++;
        // printer.print(right-left+1,array);
        if(aim == k) return array[r];
        if(aim < k) {cmps++; return select(array, left, r-1, aim);}
        else {cmps++;return select(array, r+1, right, aim-k);}
    }
    private:
    int findPivot(T* array, int left, int right){
        int size = right-left+1;
        int nGroups = std::ceil(size/5.0);
        int* medians = new int[nGroups];
        int nLast = size - 5*(nGroups-1);
        int i = 0;
        while(i<nGroups-1){
            cmps++;
            insertionsort(left+5*i, left+5*(i+1)-1, array);
            medians[i] = array[left+5*i+2];
            i++;
        }
        cmps++;
        insertionsort(left+5*i, left+5*i+nLast-1, array);
        medians[i] = array[left+5*i+nLast/2];
        Select<int> select2;
        int median = select2.select(medians, 0, nGroups-1, (nGroups/2));
        int index = left;
        swps++;
        while(index<=right && array[index]!=median){
            cmps+=2;
            index++;
        }
        cmps+=2;
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
        cmps++;
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
                cmps+=2;
                array[j]=array[j-1];
                j--;
            }
            cmps++;
            array[j]=x;
            i++;
        }
    }
};