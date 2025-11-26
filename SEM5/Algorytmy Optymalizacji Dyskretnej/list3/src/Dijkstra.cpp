#include "Dijkstra.h"
#include <queue>
#include <functional>
#include "Graph.h"

using Heap = std::priority_queue<int, std::vector<int>, std::greater<int>>; //dopoprawy

std::vector<int> Dijkstra::findAllPaths(Graph g, int start) {
    return dijkstra(g, start);
}

//pytanie jak beda dane wygladac czy od 1 czy od 0
std::pair<int, int> Dijkstra::findPath(Graph g, int start, int target) {
    return {target ,dijkstra(g, start).at(target-1)};
}

std::vector<int> Dijkstra::dijkstra(Graph g, int start) {
    Heap frontier;
    std::vector<int> d;
    std::fill(d.begin(), d.end(), INT_MAX);
    std::vector<int> parent;
    parent[start] = 0;
    d[start] = 0;
    frontier.push(start);
    while(!frontier.empty()) {
        int current = frontier.top();
        frontier.pop();
        std::vector<int> neighbours = g.getNeighbours(current);
        for(int neighbour: neighbours) {
            int new_distance = d[current]; //+ c[current][neighbour];
            if(new_distance < d[neighbour]) {
                d[neighbour] = new_distance;
                parent[neighbour] = current;
                frontier.push(neighbour);
            }
        }
    }
}