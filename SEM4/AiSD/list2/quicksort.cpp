template <class T>
class QuickSort {
    public:
    void hybridsort(int firstindex, int lastindex, int k, T* array){
        if(firstindex<lastindex){
            int pivotindex = partition(firstindex, lastindex, array);

            hybridsort(firstindex, pivotindex, k, array);
            hybridsort(pivotindex+1, lastindex, k, array);
        }
    }
    private:
    int partition(int firstindex, int lastindex, T* array){
        T pivot = array[firstindex];
        int i = firstindex - 1; 
        int j = lastindex+1;
        while(i<=j){
            do
            {
                i++;
            } while (array[i]<pivot);
            do
            {
                j--;
            } while (array[j]>pivot);
            if(i>=j) return j;
            swap(&array[i],&array[j]);
        }
        return getIndex(pivot,array, firstindex, lastindex);
    }
    void swap(T* left, T* right){
        T temp = *left;
        *left = *right;
        *right = *temp;
    }
};