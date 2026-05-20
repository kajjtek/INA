#ifndef RADIXDIJKSTRA_H
#define RADIXDIJKSTRA_H
#pragma once
#include <vector>

class Graph;

class RadixDijkstra {
public:
    using Distance = long long;

    static std::vector<Distance>
    shortestPaths(const Graph& g, int source);
};

#endif