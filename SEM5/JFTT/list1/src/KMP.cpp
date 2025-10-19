#include "../header/StringMatching.h"
#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char* argv[]){
    if(argc!=3){
        std::cerr<<"U need to input two parameters - name of file and a pattern. ./fa <filename> <pattern>"<<std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string pattern = argv[2];

    std::ifstream inputFile(filename);
    
    if(!inputFile.is_open()){
        std::cerr<<"Issues opening a file: "<<filename<<std::endl;
        return 1;
    }

    
    StringMatching matcher;
    std::string line;
    int rows = 0;

    while(std::getline(inputFile, line)){
        matcher.KMP(line,pattern, rows);
        rows++;
    }

    inputFile.close();

    return 0;
}