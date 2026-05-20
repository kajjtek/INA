#include "../include/RadixHeap.hpp"
#include "../include/Graph.h"
#include "../include/RadixDijkstra.h"

static constexpr long long INF = (1LL << 60);

std::vector<RadixDijkstra::Distance>
RadixDijkstra::shortestPaths(const Graph& g, int source) {
    int n = static_cast<int>(g.v_size-1);
    std::vector<Distance> dist(n, INF);

    dist[source] = 0;

    RadixHeap pq;
    pq.push(0, source);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u])
            continue;

        for (const auto& [v, w] : g.getNeighbours(u)) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push(dist[v], v);
            }
        }
    }

    return dist; // NRVO / move → zero copy
}
