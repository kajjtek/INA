#ifndef RADIXDIJKSTRA_H
#define RADIXDIJKSTRA_H

#include <utility>
#include "./Graph.h"
#include <list>
#include "RadixHeap.hpp"

using Bucket = std::list<int>;
using Container = std::vector<Bucket>;

class RadixDijkstra {
    public:
    std::vector<long long> findAllPaths(Graph &g, int start, long long c);
    std::pair<int, long long> findPath(Graph &g, int start, int target, long long c);
    private:
    std::vector<long long> rDijkstra(Graph &g, int start, long long c);
    int getNextBucket(int k, Container &container, long long c);
    static void updateProcedure(int current_node, RadixHeap &heap, Graph &g, std::vector<long long> &d, std::vector<int> &parent);
};

#endif