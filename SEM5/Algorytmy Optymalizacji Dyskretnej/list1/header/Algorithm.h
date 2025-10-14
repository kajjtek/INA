#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <deque>
#include <vector>
#include "Graph.h"

enum class AlgType {
    DFS, BFS
};

class Algorithm{
    public:
    std::vector<int> BFS(Graph g, int start){
        return search(g, AlgType::BFS, start);
    }
    std::vector<int> DFS(Graph g, int start){
        return search(g, AlgType::DFS, start);
    }
    std::vector<int> sortTopologicaly(Graph g);
    private:
    std::vector<int> search(Graph g, AlgType t, int start);
};

#endif