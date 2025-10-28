#include "../header/Graph.h"
#include "../header/FileReader.h"
#include "../header/Algorithm.h"
#include <iostream>
#include <optional>
#include <utility>
#include <chrono>

void printTree(Graph g, int* visited, std::vector<int> order){
    for(int v: order){
        if(visited[v]!=v){
            int u = visited[v];
            std::cout<<"("<<u+1<<","<<v+1<<")";
        } else std::cout<<v+1;
        std::cout<<std::endl;
    }
}

int main(int argc, char* argv[]){
    FileReader reader;
    Algorithm algorithm;

    if(argc!=3){
        std::cerr<<"U need to input two parameters - name of file and a pattern. ./fa <filename> <pattern>"<<std::endl;
        return 1;
    }

    std::string path = argv[1];
    char flag = (char) argv[2][0];

    Graph g;
    try {
        g = reader.createGraphFromFile(path);
    }catch(std::exception& e){
        std::cerr<<e.what()<<std::endl;
        return 1;
    }
    
    int numberOfVertices = g.v_size-1;
    int* visited = new int[numberOfVertices];
    std::fill(visited, visited+numberOfVertices, -1);

    auto start = std::chrono::high_resolution_clock::now();
    std::optional<std::pair<std::vector<int>, std::vector<int>>> option = algorithm.isBipartiate(g);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout<<"Operation took "<<elapsed.count()<<" ms to execute"<<std::endl;

    if(option.has_value()){
        std::cout<<"The graph IS bipariate"<<std::endl;
        std::pair<std::vector<int>, std::vector<int>> pair = option.value();
        std::vector<int> u1 = pair.first;
        std::vector<int> u2 = pair.second;
        if(numberOfVertices<=200){
            std::cout<<"First Set"<<std::endl;
            for(int cur: u1){
                std::cout<<cur<<", "; 
            }
            std::cout<<std::endl;
            std::cout<<"Second Set"<<std::endl;
            for(int cur: u2){
                std::cout<<cur<<", "; 
            }
            std::cout<<std::endl;
        }          
    }else std::cout<<"Graph IS NOT bipartiate"<<std::endl;


    delete[] visited;
    return 0;
}