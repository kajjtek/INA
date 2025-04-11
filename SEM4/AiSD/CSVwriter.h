#include <fstream>
#include <iostream>
#include <string>

class CSVwriter
{
private:
    std::ofstream file;
public:
    void writeFile(int n, int cmps, int swaps);
    CSVwriter(std::string name);
    ~CSVwriter();
};


void CSVwriter::writeFile(int n, int cmps, int swps){
    if(file.is_open()){
        file << n << "," << cmps << "," << swps <<"\n";
    }else{
        std::cerr << "File is not open for writing." << std::endl;
    }
}


CSVwriter::CSVwriter(std::string name)
{
    file.open(name);
    if(file.is_open()){
        file << "Size of array, Number of comparisons, Number of swaps\n";
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
