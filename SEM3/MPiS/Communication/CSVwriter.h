#include <fstream>
#include <iostream>

class CSVwriter
{
private:
    std::ofstream file;
    int tura;
    int n;
public:
    void setN(int x);
    void writeFile();
    void setResults(int tura);
    CSVwriter();
    ~CSVwriter();
};

void CSVwriter::setResults(int tura){
    this->tura=tura;
}

void CSVwriter::setN(int x){
    this->n=x;
}


void CSVwriter::writeFile(){
    if(file.is_open()){
        file << this->n << "," << this->tura << "\n";
    }else{
        std::cerr << "File is not open for writing." << std::endl;
    }
}

CSVwriter::CSVwriter()
{
    file.open("Communication_2.csv");
    if(file.is_open()){
        file << "Stations, Procedures executed\n";
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
