#ifndef RADIXHEAP_H
#define RADIXHEAP_H

#pragma once
#include <vector>
#include <cstdint>
#include <limits>
#include <cassert>

class RadixHeap {
public:
    using Key = long long;
    using U   = unsigned long long;

    RadixHeap();

    bool empty() const;
    std::size_t size() const;

    void push(Key key, int value);
    std::pair<Key, int> top();
    void pop();

private:
    static constexpr int B = 65;

    std::vector<std::pair<U,int>> buckets[B];
    U last;
    std::size_t sz;

    int bucketIndex(U x) const;
    void pull();
};


#endif