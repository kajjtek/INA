#include <random>
template <class T>
class generator {
    public:
    T* generateRandomArray(int size){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution(0,2*size-1);
        T* array = new T[size];
        for(int i=0; i<size;i++){
            array[i]=distribution(gen);
        }
        return array;
    }
    T* generateAscendingArray(int size){
        T* array = new T[size];
        for(int i=0; i<size; ++i){
            array[i]=i;
        }
        return array;
    }
    T* generateDescendingArray(int size){
        T* array = new T[size];
        for(int i=0; i<size; i++){
            array[i]=size-i;
        }
        return array;
    }
    int* copy_array(int size, int* array){
        int* temp = new int[size];
        for(int i=0; i<size; i++){
            temp[i]=array[i];
        }
        return temp;
    }
};