#include "../include/RadixHeap.hpp"

RadixHeap::RadixHeap() : last(0), sz(0) {}

bool RadixHeap::empty() const {
    return sz == 0;
}

std::size_t RadixHeap::size() const {
    return sz;
}

void RadixHeap::push(Key key, int value) {
    U x = static_cast<U>(key);
    assert(x >= last);   // monotoniczność kluczy

    int b = bucketIndex(x);
    buckets[b].emplace_back(x, value);
    ++sz;
}

std::pair<RadixHeap::Key, int> RadixHeap::top() {
    if (buckets[0].empty())
        pull();

    const auto& p = buckets[0].back();
    return { static_cast<Key>(p.first), p.second };
}

void RadixHeap::pop() {
    if (buckets[0].empty())
        pull();

    buckets[0].pop_back();
    --sz;
}

int RadixHeap::bucketIndex(U x) const {
    if (x == last) return 0;

    U diff = x ^ last;
    int msb = 63 - __builtin_clzll(diff);
    return msb + 1; // 1..64
}

void RadixHeap::pull() {
    int i = 1;
    while (i < B && buckets[i].empty())
        ++i;

    if (i >= B)
        throw std::runtime_error("RadixHeap::pull on empty heap");

    U new_last = buckets[i][0].first;
    for (const auto& p : buckets[i])
        if (p.first < new_last)
            new_last = p.first;

    last = new_last;

    for (const auto& p : buckets[i]) {
        int b = bucketIndex(p.first);
        buckets[b].push_back(p);
    }

    buckets[i].clear();
}
