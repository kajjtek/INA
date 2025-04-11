#include <utility>

template <class T>
class DualPivot {
    public:
    int* cmps = new int(0);
    int* swps = new int(0);
    arrayPrinter<T> printer;
    void sort(int firstindex, int lastindex, T* array){
        if(firstindex<lastindex){   
            std::pair<int,int> pivotindex = partition(firstindex, lastindex, array);
            int indexp = pivotindex.first;
            int indexq = pivotindex.second;

            printer.print(lastindex-firstindex+1, array);
            sort(firstindex, indexp-1, array);
            sort(indexp+1, indexq-1, array);
            sort(indexq+1, lastindex, array);
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
    std::pair<int, int> partition(int firstindex, int lastindex, T* array){
        if(array[firstindex]>array[lastindex]) swap(&array[firstindex],&array[lastindex]);
        T p = array[firstindex];
        T q = array[lastindex];

        int l = 0;
        int s = 0;

        int i = firstindex + 1; 
        int lt = firstindex +1;
        int gt = lastindex-1;
        while(i<=gt){
            if(l>s){
                if(array[i]>q){
                    (*this->cmps)++;
                    swap(&array[i],&array[gt]);
                    (*this->swps)++;
                    l++;
                    gt--;
                    continue;
                }
                else if(array[i]<p){
                    swap(&array[i],&array[lt]);
                    (*this->swps)++;
                    s++;
                    lt++;
                }
                (*this->cmps)++;
            }else{
                (*this->cmps)++;
                if(array[i]<p){
                    swap(&array[i],&array[lt]);
                    lt++;
                    s++;
                }else if(array[i]>q){
                    swap(&array[i],&array[gt]);
                    gt--;
                    l++;
                    continue;
                }else{
                    (*this->cmps)++;
                }
            }
            i++;
        }
        swap(&array[firstindex],&array[lt-1]);
        swap(&array[lastindex],&array[gt+1]);
        return std::make_pair(lt-1,gt+1);
    }
    void swap(T* left, T* right){
        T temp = *left;
        *left = *right;
        *right = temp;
    }
};