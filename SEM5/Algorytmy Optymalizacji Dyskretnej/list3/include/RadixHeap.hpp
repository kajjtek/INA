#ifndef RADIXHEAP_H
#define RADIXHEAP_H

#include <vector>
#include <list>
#include <cmath>
#include <utility>
#include <bit>
#include <algorithm>
#include <climits>
#include <stdexcept>

class RadixHeap {
    using Bucket = std::list<int>;
    public:
    RadixHeap(int c, int n, std::vector<long long> &distar): d(distar) {
        unsigned long long prod = 0;
        if (c > 0 && n > 0) prod = static_cast<unsigned long long>(c) * static_cast<unsigned long long>(n);
        int size_i = 0;
        if (prod > 0) {
            size_i = static_cast<int>(std::bit_width(prod));
        }
        this->sizeOfContainer = size_i + 1;
        this->container.resize(this->sizeOfContainer);
        this->store_place.resize(n);
        // initialize store_place entries to a safe invalid state
        for (int i = 0; i < n; ++i) {
            this->store_place[i].first = -1;
            this->store_place[i].second = this->container[0].end();
        }
        this->d_min = 0;
    }

    int extractMin() {
        int first_index = findFirstNonEmptyBucket();
        if (first_index == -1) {
            throw std::runtime_error("All buckets are empty. Cannot extract minimum.");
        }

        if (first_index > 0) {
            Bucket& firstBucket = container.at(first_index);
            auto comparator = [this](const int &a, const int &b) {
                return this->d.at(a) < this->d.at(b);
            };
            int min_node = *std::min_element(firstBucket.begin(), firstBucket.end(), comparator);
            redistribute(firstBucket, min_node);
            return extractMin();
        } else { // 0 lub 1
            Bucket& firstBucket = container.at(first_index);
            int x = firstBucket.front();
            firstBucket.pop_front();
            return x;
        }
    }

    void insert(int node, long long distance) {
        if (node < 0 || node >= static_cast<int>(d.size())) {
            return; // Invalid node index, ignore
        }
        d.at(node) = distance;
        int k = calculateBucketIndex(distance);
        store_place[node] = {k ,container[k].insert(container[k].end(), node)};
    }
    void deletion(int node) {
            if (node < 0 || node >= static_cast<int>(d.size())) return; // Safety check
        
        std::pair<int, std::list<int>::iterator>& pair = store_place[node];
        int index = pair.first;
        
        if (index >= 0 && index < static_cast<int>(container.size())) { 
            std::list<int>::iterator iter = pair.second;

            container[index].erase(iter);
        }
        store_place[node] = {-1, container[index].end()};
    }
    bool empty(){
        for(const Bucket &bucket:container) {
            if(!bucket.empty()) return false;
        }
        return true;
    }
    void decreaseKey(int node, int new_distance) {
        deletion(node);
        insert(node, new_distance);
    }
    private:
    void redistribute(Bucket &currentBucket, int min_node) {
        this->d_min = this->d.at(min_node);

        Bucket nodes_to_move;
        nodes_to_move.splice(nodes_to_move.begin(), currentBucket);

        for(int node: nodes_to_move) {
            int k = calculateBucketIndex(this->d.at(node));
            std::list<int>::iterator new_it = container[k].insert(container[k].end(), node);
            store_place[node] = {k, new_it};
        }
    }

    int calculateBucketIndex(long long distance) {
        if (distance < this->d_min) return 0; 
        unsigned long long difference = static_cast<unsigned long long>(distance-this->d_min);
        if (difference == 0ULL) return 0;

        int idx = static_cast<int>(std::bit_width(difference)) - 1;
        if (idx < 0) idx = 0;
        if (idx >= this->sizeOfContainer) idx = this->sizeOfContainer - 1;
        return idx;
    }

    int findFirstNonEmptyBucket() {
        for (int cur_index = 0; cur_index < this->sizeOfContainer; ++cur_index) {
            if (!container.at(cur_index).empty()) {
                return cur_index;
            }
        }
        return -1;
    }

    std::vector<long long> &d;
    std::vector<std::pair<int ,std::list<int>::iterator>> store_place;
    long long d_min;
    std::vector<Bucket> container;
    int sizeOfContainer;
};

#endif