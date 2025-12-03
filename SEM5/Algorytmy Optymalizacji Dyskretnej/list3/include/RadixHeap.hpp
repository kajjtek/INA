#ifndef RADIXHEAP_H
#define RADIXHEAP_H

#include <vector>
#include <list>
#include <cmath>
#include <utility>
#include <bit>
#include <algorithm>
#include <climits>

class RadixHeap {
    using Bucket = std::list<int>;
    public:
    RadixHeap(int c, int n, std::vector<long long> &distar): d(distar) {
        int size_i = static_cast<int>(std::ceil(std::log(c*n)));
        this->sizeOfContainer=size_i+1;
        this->container.resize(this->sizeOfContainer);
        this->store_place.resize(n);
    }
    int extractMin() {
        int first_index = findFirstEmptyBucket();
        if(first_index>1) {
            Bucket& firstBucket = container.at(first_index);
            auto comparator = [this](const int &a, const int &b) {
                return this->d.at(a) < this->d.at(b);
                };
            int min_node = *std::min_element(firstBucket.begin(), firstBucket.end(), comparator);
            distribute(firstBucket, min_node);
            return extractMin();
        } else { // 0 lub 1
            Bucket& firstBucket = container.at(first_index);
            int x = firstBucket.front();
            firstBucket.pop_front();
            return x;
        }

    }
    void insert(int node, long long distance) {
        const long long INF = LLONG_MAX;
        if(d.at(node)!=INF) { // already present -> remove
            deletion(node);
        }
        d.at(node) = distance;
        int k = calculateBucketIndex(distance);
        store_place[node] = {k ,container[k].insert(container[k].end(), node)};
    }
    void deletion(int node) {
        std::pair<int, std::list<int>::iterator> pair = store_place[node];
        int index = pair.first;
        if(index < 0) return;
        std::list<int>::iterator iter = pair.second;
        if(iter != container[index].end()) container[index].erase(iter);
        store_place[node] = {-1, container[index].end()};
    }
    void decreaseKey(int node, int new_distance) {
        deletion(node);
        insert(node, new_distance);
    }
    bool empty(){
        for(const Bucket &bucket:container) {
            if(!bucket.empty()) return false;
        }
        return true;
    }
    private:
    void distribute(Bucket &currentBucket, int min_node) {
        this->d_min = this->d.at(min_node);
        
        std::vector<int> nodes_to_move(currentBucket.begin(), currentBucket.end());
        currentBucket.clear(); 

        for(int node : nodes_to_move) {
            insert(node, this->d.at(node)); 
        }
    }
    int calculateBucketIndex(int distance) {
        int difference = std::abs(this->d_min-distance);
        return std::bit_width(static_cast<unsigned int>(difference));
    }
    int findFirstEmptyBucket() {
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