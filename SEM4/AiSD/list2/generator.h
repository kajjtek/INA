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
        int min = 0;
        std::random_device rd;
        std::mt19937 gen(rd());
        T* array = new T[size];
        for(int i=0; i<size; i++){
            std::uniform_int_distribution<> distribution(min,2*size-1);
            array[i]=distribution(gen);
            min=array[i];
        }
        return array;
    }
    T* generateDescendingArray(int size){
        int max = 2*size -1;
        std::random_device rd;
        std::mt19937 gen(rd());
        T* array = new T[size];
        for(int i=0; i<size; i++){
            std::uniform_int_distribution<> distribution(0,max);
            array[i]=distribution(gen);
            max=array[i];
        }
        return array;
    }
};