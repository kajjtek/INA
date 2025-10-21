#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <deque>
#include <vector>
#include "Graph.h"
#include <optional>
#include <utility>

enum class AlgType {
    DFS, BFS
};

class Algorithm{
    public:
    std::vector<int> BFS(Graph g, int start, int* visited, bool treeable){
        return search(g, AlgType::BFS, start, visited, treeable);
    }
    std::vector<int> DFS(Graph g, int start, int* visited, bool treeable){
        return search(g, AlgType::DFS, start, visited, treeable);
    }
    std::vector<int> sortTopologicaly(Graph g);
    std::vector<std::vector<int>> SSC(Graph g);
    std::optional<std::pair<std::vector<int>, std::vector<int>>> isBipartiate(Graph g);
    private:
    std::vector<int> search(Graph g, AlgType t, int start, int* visited, bool treeable);
};

#endif