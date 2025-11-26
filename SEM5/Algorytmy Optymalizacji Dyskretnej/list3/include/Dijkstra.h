#ifndef DIJSKTRA_H
#define DIJSKTRA_H

#include <utility>
#include "./Graph.h"

class Dijkstra {
    public:
    std::vector<int> findAllPaths(Graph g, int start);
    std::pair<int, int> findPath(Graph g, int start, int target);
    private:
    std::vector<int> dijkstra(Graph g, int start);
};

#endif