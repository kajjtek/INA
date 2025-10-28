#include "../header/Graph.h"
#include "../header/FileReader.h"
#include "../header/Algorithm.h"
#include <iostream>
#include <optional>
#include <utility>


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

    std::cout<<"-----------------------------------------"<<std::endl;
    std::cout<<"B F S"<<std::endl;
    std::cout<<"-----------------------------------------"<<std::endl;
    
    int numberOfVertices = g.v_size-1;
    int* visited = new int[numberOfVertices];
    std::fill(visited, visited+numberOfVertices, -1);

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<int> order1 = algorithm.BFS(g, 0, visited, true);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout<<"Operation took "<<elapsed.count()<<" ms to execute"<<std::endl;

    for(int curr:order1){
        std::cout<<curr+1<<", ";
    }
    std::cout<<std::endl;

    if(flag=='t') printTree(g, visited, order1);

    std::cout<<"-----------------------------------------"<<std::endl;
    std::cout<<"D F S"<<std::endl;
    std::cout<<"-----------------------------------------"<<std::endl;

    std::fill(visited, visited+numberOfVertices, -1);

    start = std::chrono::high_resolution_clock::now();
    order1 = algorithm.DFS(g, 0, visited, true);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout<<"Operation took "<<elapsed.count()<<" ms to execute"<<std::endl;

    for(int curr:order1){
        std::cout<<curr+1<<", ";
    }
    std::cout<<std::endl;

    if(flag=='t') printTree(g, visited, order1);

    delete[] visited;
    return 0;
}