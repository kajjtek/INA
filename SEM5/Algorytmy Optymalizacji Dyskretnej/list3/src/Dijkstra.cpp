#include "../include/Dijkstra.h"
#include <queue>
#include <functional>
#include "../include/Graph.h"
#include <vector>
#include <climits>
#include <iostream>

using PII_LL = std::pair<long long,int>;
using Heap = std::priority_queue<PII_LL, std::vector<PII_LL>, std::greater<PII_LL>>;

std::vector<long long> Dijkstra::findAllPaths(Graph &g, int start) {
    return dijkstra(g, start);
}

// Note: this implementation treats start/target as 0-based indices.
// If your inputs are 1-based, subtract 1 before calling.
std::pair<int, long long> Dijkstra::findPath(Graph &g, int start, int target) {
    auto d = dijkstra(g, start);
    if(target < 0 || target >= (int)d.size()) return {target, LLONG_MAX};
    return {target, d.at(target)};
}

std::vector<long long> Dijkstra::dijkstra(Graph &g, int start) {
    int n = 0;
    if (g.v_size > 0) n = g.v_size - 1;

    const long long INF = LLONG_MAX;
    std::vector<long long> d(n, INF);
    std::vector<int> parent(n, -1);

    if(start < 0 || start >= n) return d; // invalid start

    Heap frontier;
    d[start] = 0;
    parent[start] = -1;
    frontier.push({0LL, start});

    while(!frontier.empty()) {
        std::pair<long long, int> current_pair = frontier.top();
        long long current_distance = current_pair.first;
        int current = current_pair.second;
        frontier.pop();
        if (current_distance > d[current]) {
            continue;
        }
        const std::vector<std::pair<long long, int>>& neighbours = g.getNeighbours(current);
        for(const std::pair<long long, int> &pair_neighbour: neighbours) {
            int neighbour = pair_neighbour.second;
            long long weight = pair_neighbour.first;
            long long new_distance = d[current] + weight;
            if(new_distance < d[neighbour]) {
                d[neighbour] = new_distance;
                parent[neighbour] = current;
                frontier.push({new_distance, neighbour});
            }
        }
    }

    return d;
}