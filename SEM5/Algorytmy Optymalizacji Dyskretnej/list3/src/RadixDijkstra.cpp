#include "RadixDijkstra.h"
#include <queue>
#include <functional>
#include "Graph.h"
#include "RadixHeap.hpp"
#include <vector>
#include <climits>

using Bucket = std::list<int>;
using Container = std::vector<Bucket>;

std::vector<long long> RadixDijkstra::findAllPaths(Graph &g, int start, int c) {
    return rDijkstra(g, start, c);
}
std::pair<int, long long> RadixDijkstra::findPath(Graph &g, int start, int target, int c) {
    return {target, rDijkstra(g, start, c).at(target-1)};
}

std::vector<long long> RadixDijkstra::rDijkstra(Graph &g, int start, int c) {
    int n = (g.v_size > 0) ? g.v_size - 1 : 0;
    const long long INF = LLONG_MAX;
    std::vector<long long> d(n, INF);
    std::vector<int> parent(n, -1);
    RadixHeap heap(c, n, d); // heap will reference the same d vector
    heap.insert(start, 0LL);
    while(!heap.empty()) {
        int cur_node = heap.extractMin();
        if(cur_node < 0 || cur_node >= n) continue;
        updateProcedure(cur_node, heap, g, d, parent);
    }
    return d;
}

void RadixDijkstra::updateProcedure(int current_node, RadixHeap &heap, Graph &g, std::vector<long long> &d, std::vector<int> &parent) {
    std::vector<std::pair<int,int>> neighbours = g.getNeighbours(current_node);
    for(std::pair<int, int> neighbour_pair: neighbours) {
        int neighbour = neighbour_pair.second;
        int weight = neighbour_pair.first;
        long long current_distance = d[neighbour];
        long long new_distance = d[current_node] + weight;
        if(new_distance<current_distance) {
            heap.insert(neighbour, new_distance);
        }
    }
}