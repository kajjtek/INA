#ifndef DIAL_H
#define DIAL_H

#include <utility>
#include <list>
#include "./Graph.h"
#include "RadixHeap.hpp"

using Bucket = std::list<int>;
using Container = std::vector<Bucket>;

class Dial {
    public:
    std::vector<long long> findAllPaths(Graph &g, int start, long long c);
    std::pair<int, long long> findPath(Graph &g, int start, int target, long long c);
    private:
    std::vector<long long> dial(Graph &g, int start, long long c);
};

#endif