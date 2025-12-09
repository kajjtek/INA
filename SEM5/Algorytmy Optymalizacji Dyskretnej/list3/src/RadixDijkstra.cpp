#include "../include/RadixDijkstra.h"
#include <queue>
#include <functional>
#include "../include/Graph.h"
#include "../include/RadixHeap.hpp"
#include <vector>
#include <climits>

using Bucket = std::list<int>;
using Container = std::vector<Bucket>;

std::vector<long long> RadixDijkstra::findAllPaths(Graph &g, int start, int c) {
    return rDijkstra(g, start, c);
}
std::pair<int, long long> RadixDijkstra::findPath(Graph &g, int start, int target, int c) {
    auto d = rDijkstra(g, start, c);
    if (target < 0 || target >= static_cast<int>(d.size())) return {target, LLONG_MAX};
    return {target, d.at(target)};
}

std::vector<long long> RadixDijkstra::rDijkstra(Graph &g, int start, int c) {
    int n = (g.v_size > 0) ? g.v_size - 1 : 0;
    const long long INF = LLONG_MAX;
    std::vector<long long> d(n, INF);
    std::vector<int> parent(n, -1);
    RadixHeap heap(c, n, d); // heap will reference the same d vector
    if (start < 0 || start >= n) return d;
    d[start] = 0;
    parent[start] = -1;
    heap.insert(start, 0LL);
    while(!heap.empty()) {
        int cur_node = heap.extractMin();
        if(cur_node < 0 || cur_node >= n) continue;
        updateProcedure(cur_node, heap, g, d, parent);
    }
    return d;
}

void RadixDijkstra::updateProcedure(int current_node, RadixHeap &heap, Graph &g, std::vector<long long> &d, std::vector<int> &parent) {
    if (current_node < 0 || current_node >= static_cast<int>(d.size())) return;
    if (d[current_node] == LLONG_MAX) return; // unreachable

    std::vector<std::pair<int,int>> neighbours = g.getNeighbours(current_node);
    for (const std::pair<int, int>& neighbour_pair: neighbours) {
        int neighbour = neighbour_pair.second;
        int weight = neighbour_pair.first;
        if (neighbour < 0 || neighbour >= static_cast<int>(d.size())) continue;
        long long current_distance = d[neighbour];
        long long new_distance = d[current_node];
        // avoid overflow
        if (new_distance != LLONG_MAX) new_distance += weight;
        if(current_distance==LLONG_MAX) {
            heap.insert(neighbour, new_distance);
            continue;
        }
        else if (new_distance < current_distance) {
            heap.decreaseKey(neighbour, new_distance);
        }
    }
}