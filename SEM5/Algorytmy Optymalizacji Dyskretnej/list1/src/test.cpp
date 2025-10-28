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
    
    int numberOfVertices = g.v_size-1;
    int* visited = new int[numberOfVertices];
    std::fill(visited, visited+numberOfVertices, -1);
    std::vector<int> order1 = algorithm.BFS(g, 0, visited, true);

    for(int curr:order1){
        std::cout<<curr+1<<", ";
    }
    std::cout<<std::endl;

    if(flag=='t') printTree(g, visited, order1);

    std::fill(visited, visited+numberOfVertices, -1);
    order1 = algorithm.DFS(g, 0, visited, true);

    for(int curr:order1){
        std::cout<<curr+1<<", ";
    }
    std::cout<<std::endl;

    if(flag=='t') printTree(g, visited, order1);

    try {
        order1 = algorithm.sortTopologicaly(g, true);
        if(order1.size()>0) std::cout<<"The graph has no cycles"<<std::endl;
        if(numberOfVertices<=200){
            for(int current: order1){
                std::cout<<current+1<<", ";
            }
            std::cout<<std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr<<e.what()<<std::endl;
        std::cout<<"GRAPH HAS A CYCLE"<<std::endl;
    }

    try {
        std::vector<std::vector<int>> sscs = algorithm.SSC(g);
        for(std::vector<int> component: sscs){
            std::cout<<"Size of a component: "<<component.size()<<std::endl;
            if(numberOfVertices<=200){
                for(int x:component){
                    std::cout<<x+1<<", ";
                }
                std::cout<<std::endl;
            }
        }
    }catch(...) {}

    std::optional<std::pair<std::vector<int>, std::vector<int>>> option = algorithm.isBipartiate(g);
    if(option.has_value()){
        std::cout<<"The graph is bipariate"<<std::endl;
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