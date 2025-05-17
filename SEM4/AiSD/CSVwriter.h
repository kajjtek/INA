#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

class CSVwriter
{
private:
    std::ofstream file;
public:
    void writeFile(int n, int cmps, double swaps);
    void writeFileEX3(int n, int cmps, int swaps, int k);
    void writeFileWithTime(int n, int cmps, int swaps, double time);
    CSVwriter(std::string name);
    void header(std::string row);
    ~CSVwriter();
};


void CSVwriter::writeFile(int n, int cmps, double swps){
    if(file.is_open()){
        file << n << "," << cmps << "," << swps <<"\n";
    }else{
        std::cerr << "File is not open for writing." << std::endl;
    }
}

void CSVwriter::writeFileWithTime(int n, int cmps, int swps, double time){
    if(file.is_open()){
        file << n << "," << cmps << "," << swps <<"\n";
    }else{
        std::cerr << "File is not open for writing." << std::endl;
    }
}

void CSVwriter::writeFileEX3(int n, int cmps, int swps, int k){
    if(file.is_open()){
        file << n << "," << cmps << "," << swps <<"\n";
    }else{
        std::cerr << "File is not open for writing." << std::endl;
    }
}

void CSVwriter::header(std::string row){
    if(file.is_open()){
        file << row << "\n";
    }else{
        std::cerr << "file aint open for writing" <<std::endl;
    }
}


CSVwriter::CSVwriter(std::string name)
{
    file.open(name);
    if(file.is_open()){
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
