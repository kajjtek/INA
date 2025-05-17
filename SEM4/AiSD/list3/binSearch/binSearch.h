#include <random>

template <class T>
class binSearch
{
private:
    char choice;
    int getIndex(int left, int right, char choice){
        if(choice=='m') return middleIndex(left,right);
        return randomIndex(left,right);
    }
    int middleIndex(int left, int right){
        return (right+left)/2;
    }
    int randomIndex(int left, int right){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution(left,right);
        return distribution(gen);
    }
public:
    binSearch(char c){
        this->choice = c;
    };
    int cmps = 0;
    int find(T* arr, int left, int right, T target){
        int size = right - left + 1;
        cmps++;
        if(size < 1) return 0;
        cmps++;
        if(size == 1){
            cmps++;
            if(arr[left] == target){
                return 1;
            }
            return 0;
        }
        int mid = getIndex(left,right, this->choice);
        cmps++;
        if(arr[mid] == target) return 1;
        cmps++;
        if(arr[mid] < target) return find(arr, mid+1, right, target);
        cmps++;
        return find(arr, left, mid-1, target);
    }
};

