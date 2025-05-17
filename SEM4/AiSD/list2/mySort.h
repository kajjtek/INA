#include <vector>
#include <deque>
#include <stdio.h>
#include <iostream>
#include "../arrayPrinter.h"

template <class T>
class mySort{
    public:
    int* cmps = new int(0);
    int* swps = new int(0);
    int* sort(int* array, int size){
        std::deque<std::vector<T> > runs = findSequences(array, size);

        if(runs.size()==1){
            return array;
        }

        while(runs.size()>1){
            int size = runs.size();
            int i =0;
            while(i<size){
                if(i+1<size){
                    std::vector<T> x = runs.front();
                    runs.pop_front();
                    std::vector<T> y = runs.front();
                    runs.pop_front();
                    runs.push_back(merge(x,y));
                }else{
                    std::vector<T> x = runs.front();
                    runs.pop_front();
                    runs.push_back(x);
                }
                i+=2;
            }
        }
        std::vector<T> result = runs.front();
        std::copy(result.begin(), result.end(), array);
        return array;
    }
    bool isSorted(T* sorted, T* copy, int size){
        std::sort(copy, copy+size);
        for(int i=0; i<size; i++){
            if(sorted[i]!=copy[i]) return false;
        }
        return true;
    }
    ~mySort(){
        delete cmps;
        delete swps;
    }
    private:
    int* getStdArray(std::vector<T> deque){
        int* array = new int[deque.size()];
        for(int i=0; i<deque.size();++i){
            array[i]=deque.at(i);
        }
        return array;
    }
    std::deque<std::vector<T> > findSequences(T* array, int size){
        std::deque<std::vector<T> > runs;
        int i=0;
        while(i<size){
            std::vector<T> run(1,array[i]);
            (*this->cmps)++;
            while(i<size-1 && array[i]<=array[i+1]){
                run.push_back(array[++i]);
            }
            runs.push_back(run);
            i++;
        }
        return runs;
    }


    std::vector<T> merge(std::vector<T> left, std::vector<T> right){
        std::vector<T> temp;

        int i=0, j=0;

        while(i<left.size() && j<right.size()){
            (*this->swps)++;
            (*this->cmps)++;
            if(left[i]<=right[j]){
                temp.push_back(left[i++]);
            }else{
                (*this->cmps)++;
                temp.push_back(right[j++]);
                }
        }

        while(i<left.size()){
            (*this->swps)++;
            temp.push_back(left[i++]);
        }
        
        while(j<right.size()){
            (*this->swps)++;
            temp.push_back(right[j++]);
        }

        arrayPrinter<int> printer;
        printer.print(left.size()+right.size(),temp.data());

        return temp;
    }
};