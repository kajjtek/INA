#include <fstream>
#include <iostream>

class CSVwriter
{
private:
    std::ofstream file;
    int size;
    int n;
    int* array;
    int Sn;
public:
    void setArray(int* container);
    void setN(int x);
    void setSn(int x);
    void writeFile(int k, double Ln);
    CSVwriter();
    ~CSVwriter();
};

void CSVwriter::setArray(int* container){
    this->array=container;
}

void CSVwriter::setN(int x){
    this->n=x;
    this->size=2*n+1;
}

void CSVwriter::setSn(int x){
    this->Sn=x;
}

void CSVwriter::writeFile(int k, double Ln){
    if(file.is_open()){
        file << k <<","<<Ln<<"\n";
    }else{
        std::cerr << "File is not open for writing." << std::endl;
    }
}

CSVwriter::CSVwriter()
{
    file.open("exercise5_3_10000.csv");
    if(file.is_open()){
            file << "k, Ln";
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
