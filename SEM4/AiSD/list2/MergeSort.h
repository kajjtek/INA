#include <deque>

template <class T>
class MergeSort{
    public:
    int* cmps = new int(0);
    int* swps = new int(0);
    T* sort(T* array, int left, int right){
        int mid = left+((right-left)/2);
        int nl = mid - left +1;
        int nr = right-mid;

        if(left==right){
            T* result = new T[1];
            result[0]=array[left];
            return result;
        } 
        T* b = sort(array, left, mid);
        T* c = sort(array, mid+1, right); 

        T* merged = merge(b, c, nl, nr);
        delete[] b;
        delete[] c;
        return merged;
    }
    bool isSorted(T* sorted, T* copy, int size){
        std::sort(copy, copy+size);
        for(int i=0; i<size; i++){
            if(sorted[i]!=copy[i]) return false;
        }
        return true;
    }
    private:
    T* merge(T* left, T* right, int sizeleft, int sizeright){
        T* result = new T[sizeleft+sizeright];

        int i=0, j=0, k=0;
        while(i<sizeleft && j<sizeright){
            (*this->swps)++;
            (*this->cmps)++;
            if(left[i]<=right[j]){
                result[k++] = left[i++];
            }else{
                (*this->cmps)++;
                result[k++] = right[j++];
            }
        }

        while (i<sizeleft)
        {
            (*this->swps)++;
            result[k++]=left[i++];
        }
        
        while (j<sizeright)
        {
            (*this->swps)++;
            result[k++]=right[j++];
        }

        return result;
    }
};