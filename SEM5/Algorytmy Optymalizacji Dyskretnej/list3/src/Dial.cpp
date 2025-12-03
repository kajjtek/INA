#include "Dial.h"
#include <list>
#include <vector>

using Bucket = std::list<int>;
using Container = std::vector<Bucket>;

std::vector<long long> Dial::findAllPaths(Graph &g, int start, int c) {
    return dial(g, start, c);
}
std::pair<int, long long> Dial::findPath(Graph &g, int start, int target, int c) {
    return {target, dial(g, start, c).at(target-1)};
}

std::vector<long long> Dial::dial(Graph &g, int start, int c) {
    int bucket_count = c+1;
    Container container(bucket_count);
    int n = (g.v_size > 0) ? g.v_size-1 : 0;
    const long long INF = LLONG_MAX;
    std::vector<long long> d(n, INF);
    std::vector<int> parent(n, -1);
    std::vector<std::list<int>::iterator> node_iterator(n);
    if(start < 0 || start >= n) return d;
    d[start] = 0;
    int current_bucket_index = -1;
    node_iterator[start] = container.at(0).insert(container.at(0).end(), start);
    int count = 0;
    while(count < n) {
        current_bucket_index = getNextBucket(current_bucket_index, container, bucket_count);
        Bucket &current_bucket = container.at(current_bucket_index);
        while(!current_bucket.empty()) {
            int first_node = current_bucket.front();
            updateProcedure(first_node, container, g, node_iterator, d, bucket_count, parent);
            current_bucket.pop_front();
            count++;
        }
    }
    return d;
}

int Dial::getNextBucket(int k, Container &container, int c) {
    int k_count = k;
    k_count = ++k_count % c;
    Bucket &current_bucket = container.at(k_count);
    while(current_bucket.empty()) {
        k_count = ++k_count%c;
        current_bucket = container.at(k_count);
    }
    return k_count;
}

void Dial::updateProcedure(int current_node, Container &container, Graph &g, std::vector<std::list<int>::iterator> &node_iterator, std::vector<long long> &d, int c, std::vector<int> &parent) {
    std::vector<std::pair<int,int>> neighbours = g.getNeighbours(current_node);
    for(std::pair<int, int> neighbour_pair: neighbours) {
        int neighbour = neighbour_pair.second;
        int weight = neighbour_pair.first;
        long long current_distance = d[neighbour];
        long long new_distance = d[current_node] + weight;
        int new_bucket = (int)(new_distance % c);
        if(new_distance < current_distance) {
            if(current_distance != LLONG_MAX)  {
                container.at((int)(current_distance % c)).erase(node_iterator[neighbour]);  
            }
            node_iterator[neighbour] = container.at(new_bucket).insert(container.at(new_bucket).end(), neighbour);
            d[neighbour] = new_distance;
            parent[neighbour] = current_node;
        }
    }
}