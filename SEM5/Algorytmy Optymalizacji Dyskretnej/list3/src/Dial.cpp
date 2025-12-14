#include <vector>
#include <algorithm>
#include <climits>
#include <iostream>
#include "../include/Graph.h"
#include "../include/Dial.h"

using Distance = long long;
using Node = int;


using Edge = std::pair<Node, Distance>;
const Distance INF = LLONG_MAX;

std::vector<long long> Dial::findAllPaths(Graph &g, int start, long long c) {
    return dial(g, start, c);
}
std::pair<int, long long> Dial::findPath(Graph &g, int start, int target, long long c) {
    return {target, dial(g, start, c).at(target-1)};
}

std::vector<long long> Dial::dial(Graph &g, int start, long long c) {
    int n = static_cast<int>(g.v_size-1);

    std::vector<Distance> dist(n, INF);
    dist[start] = 0;

    int binCount = static_cast<int>(std::max(1LL, c + 1));
    
    std::vector<std::vector<Node>> buckets(binCount);

    int currentBucket = 0;
    buckets[0].push_back(start);

    while (true) {
        int steps = 0;
        while (buckets[currentBucket].empty() && steps < binCount) {
            currentBucket = (currentBucket + 1) % binCount;
            steps++;
        }

        if (steps == binCount && buckets[currentBucket].empty()) {
            break;
        }

        Node u = buckets[currentBucket].back();
        buckets[currentBucket].pop_back();

        Distance du = dist[u];

        std::vector<std::pair<long long, int>> edges = g.getNeighbours(u);
        for (std::pair<long long, int> &pair : edges) {
            Node v = pair.second;
            Distance weight = pair.first; 

            if (du == INF) continue; 

            Distance newDist = du + weight;
            Distance dv = dist[v];

            if (newDist < dv) {
               
                dist[v] = newDist;
                
                int newBucketIdx = static_cast<int>(newDist % binCount);
                
                buckets[newBucketIdx].push_back(v);
            }
        }
    }

    return dist;
}