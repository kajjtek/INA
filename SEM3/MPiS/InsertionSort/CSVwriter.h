#include <fstream>
#include <iostream>

class CSVwriter
{
private:
    std::ofstream file;
    int compareNum, swapNum;
    int n;
public:
    void setN(int x);
    void writeFile();
    void setResults(int comp, int swap);
    CSVwriter();
    ~CSVwriter();
};

void CSVwriter::setResults(int comp, int swap){
    this->compareNum=comp;
    this->swapNum=swap;
}

void CSVwriter::setN(int x){
    this->n=x;
}


void CSVwriter::writeFile(){
    if(file.is_open()){
        file << this->n << "," << this->compareNum << "," << this->swapNum << "\n";
    }else{
        std::cerr << "File is not open for writing." << std::endl;
    }
}

CSVwriter::CSVwriter()
{
    file.open("InsertionSimResults.csv");
    if(file.is_open()){
        file << "Elements in Permutation, Comparisons, Swaps\n";
    }else{
        throw std::ios_base::failure("Failed to open file: exercise2.csv");
    }
}

CSVwriter::~CSVwriter()
{
    if(file.is_open()){
        file.close();
    }
}
