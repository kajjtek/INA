template <class T>
class arrayPrinter{
    public:
    void print(int size,T* array){
        std::cout << "[";
        for(int i=0; i<size; i++){
            std::cout<<array[i];
            if(i<size-1){
                std::cout<<",";
            }
        }
        std::cout << "]" <<std::endl;
    }
    void printStats(int cmps, int swps){
        std::cout<<"STATS OF THE SORTING ALGORITHM"<<std::endl;
        std::cout<<"---------------------------"<<std::endl;
        std::cout<<"Number of comparisons: "<<cmps<<std::endl;
        std::cout<<"Number of swaps: "<<swps<<std::endl;
        std::cout<<"---------------------------"<<std::endl;
    };
};