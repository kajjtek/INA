#include <vector>
using namespace std;

class Sort
{
  public:
  int compareNum, swapNum;
  vector<int>* array;
  void InsertionSort() ;
  void setArrayInt(vector<int>& vector);
  int getComparisons();
  int getSwaps();
};

void Sort::InsertionSort(){
    int j, key;
    compareNum=0;
    swapNum=0;
    for (int i = 1; i < array->size(); i++)
    {
        key = array->at(i);
        j=i-1;
        while (j>=0 && array->at(j)>key)
        {
            compareNum++;
            array->at(j+1)=array->at(j);
            swapNum++;
            j--;
        }


        if(j>0){
            compareNum++;
        }

        array->at(j+1)=key;
    }
    
}

int Sort::getComparisons(){
    return compareNum;
}

int Sort::getSwaps(){
    return swapNum;
}

void Sort::setArrayInt(vector<int>& vector){
    this->array=&vector;
}