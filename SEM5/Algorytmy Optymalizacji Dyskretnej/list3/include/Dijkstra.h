#ifndef DIJSKTRA_H
#define DIJSKTRA_H

#include <utility>
#include "./Graph.h"

class Dijkstra {
    public:
    std::vector<long long> findAllPaths(Graph g, int start);
    std::pair<int, long long> findPath(Graph g, int start, int target);
    private:
    std::vector<long long> dijkstra(Graph g, int start);
};

#endif