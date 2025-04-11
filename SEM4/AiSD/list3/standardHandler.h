#include <iostream>
template <class T>
class standardHandler{
    public:
    int* copy_array(int size, int* array){
        int* temp = new int[size];
        for(int i=0; i<size; i++){
            temp[i]=array[i];
        }
        return temp;
    }
    int* load_array(int* size){
        std::cin.read(reinterpret_cast<char*>(size),sizeof(size));
        int* array = new int[*size];
        for(int i=0; i<*size;i++){
            std::cin.read(reinterpret_cast<char*>(&array[i]),sizeof(array[i]));
        }
        return array;
    }

};