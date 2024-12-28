#include "Results.h"
#include <fstream>
#include <iostream>

class CSVwriter
{
private:
    std::ofstream file;
    Results results;
    int n;
public:
    void setN(int x);
    void writeFile();
    void writeFileEX3();
    void setResults(const Results& x);
    CSVwriter(int x);
    ~CSVwriter();
};

void CSVwriter::setN(int x){
    this->n=x;
}

void CSVwriter::writeFileEX3(){
     if(file.is_open()){
        file << this->n << "," << results.returnMaxValue() << std::endl;
    }else{
        std::cerr << "File is not open for writing." << std::endl;
    }
}

void CSVwriter::writeFile(){
    if(file.is_open()){
        file << this->n << "," << results.returnfirstCollision() << "," << results.returnemptyBoxes() << "," << results.returnatLeastOne() << "," << results.returnatLeastTwo() << "," << results.returnDifference() << "\n";
    }else{
        std::cerr << "File is not open for writing." << std::endl;
    }
}

void CSVwriter::setResults(const Results& x){
    this->results=x;
}

CSVwriter::CSVwriter(int x)
{
    file.open("exercise3_2.csv");
    if(file.is_open()){
        if(x==0){
            file << "Liczba urn, Pierwsza kolizja, Puste urny po n rzutach, Przynajmniej jedna kulka w kazdej urnie, Przynajmiej dwie kulki w kazdej urnie, Roznica\n";
        }
        if(x==1){
            file <<"BoxesNum, MAXValue\n";
        }
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
