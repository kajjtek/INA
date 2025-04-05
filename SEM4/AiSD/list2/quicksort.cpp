template <class T>
class QuickSort {
    public:
    void sort(T* array, int left, int right){
        int pivotindex = partition(right-left+1, array);

        sort(array, left, pivotindex-1);
        sort(array, pivotindex+1, right);
    }
    private:
    int partition(int size, T* array){
        T pivot = array[0]
        int i = 0; 
        int j = size-1;
        while(i<=j){
            while(array[i]<=pivot) {i++};
            while(array[j]>=pivot) {j++};
            if(i>j) continue;
            swap(&array[i],&array[j])
            i++;
            j++;
        }
    }
    void swap(T* left, T* right){
        T temp = *left;
        *left = *right;
        *right = *temp;
    }
};