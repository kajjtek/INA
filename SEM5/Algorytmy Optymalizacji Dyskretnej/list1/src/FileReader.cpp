#include "../header/FileReader.h"
#include <fstream>
#include <iostream>

Graph FileReader::createGraphFromFile(std::string path){
    std::ifstream inputFile(path);

    if(!inputFile.is_open()){
        throw std::runtime_error("Nie można otworzyć pliku: " + path);
    }

    char type;
    int n, m;

    if (!(inputFile >> type)) {
        throw std::runtime_error("Błąd wczytywania flagi (D/U).");
    }

    if (!(inputFile >> n)) {
        throw std::runtime_error("Błąd wczytywania liczby wierzchołków (n).");
    }
    
    if (!(inputFile >> m)) {
        throw std::runtime_error("Błąd wczytywania liczby krawędzi (m).");
    }

    int numberOfEdges = m;

    std::vector<std::pair<int, int>> edges;
    int u, v;
    for(int i=0;i<numberOfEdges; i++){
        if (!(inputFile >> u >> v)) {
            throw std::runtime_error("Błąd wczytywania krawędzi (u v). Za mało krawędzi w pliku.");
        }

        edges.push_back({u,v});
        if(type=='U') edges.push_back({v,u});
    }
    
    Graph g(n, type, edges);
    return g;
}