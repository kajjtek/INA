#include "Dijkstra.h"
#include <queue>
#include <functional>
#include "Graph.h"
#include <vector>

using Heap = std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int>>, std::greater<std::pair<long long,int>>>; //dopoprawy

std::vector<long long> Dijkstra::findAllPaths(Graph g, int start) {
    return dijkstra(g, start);
}

//pytanie jak beda dane wygladac czy od 1 czy od 0
std::pair<int, long long> Dijkstra::findPath(Graph g, int start, int target) {
    return {target ,dijkstra(g, start).at(target-1)};
}

std::vector<long long> Dijkstra::dijkstra(Graph g, int start) {
    Heap frontier;
    std::vector<long long> d(g.v_size-1, __LONG_LONG_MAX__);
    std::vector<int> parent(g.v_size-1, 0);
    d[start] = 0;
    frontier.push({0, start});
    while(!frontier.empty()) {
        std::pair<long long, int> current_pair = frontier.top();
        long long current_distance = current_pair.first;
        int current = current_pair.second;
        if (current_distance > d[current]) {
            continue;
        }
        frontier.pop();
        std::vector<std::pair<int, int>> neighbours = g.getNeighbours(current);
        for(std::pair<int, int> pair_neighbour: neighbours) {
            int neighbour = pair_neighbour.second;
            int weight = pair_neighbour.first;
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