#ifndef DIAL_H
#define DIAL_H

#include <utility>
#include "./Graph.h"

using Bucket = std::list<int>;
using Container = std::vector<Bucket>;

class Dial {
    public:
    std::vector<long long> findAllPaths(Graph &g, int start, int c);
    std::pair<int, long long> findPath(Graph &g, int start, int target, int c);
    private:
    std::vector<long long> dial(Graph &g, int start, int c);
    int getNextBucket(int k, Container &container, int c);
    static void updateProcedure(int current_node, Container &container, Graph &g, std::vector<std::list<int>::iterator> &node_iterator, std::vector<long long> &d, int c, std::vector<int> &parent);
};

#endif