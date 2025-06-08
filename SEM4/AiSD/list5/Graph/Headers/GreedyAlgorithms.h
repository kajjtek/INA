#ifndef GREEDYALGORITHMS_H
#define GREEDYALGORITHMS_H
#include "Graph.h"
#include "DSU.h"

class GreedyAlgorithms {
    public:
    Graph prism(Graph &g);
    Graph Kruskal(Graph &g);
    int informationPassing(Graph &g);
    private:
    void setupSizeMap(std::unordered_map<Node*, int> &map, Node* root);
    int calculateSize(std::unordered_map<Node*, int> &map, Node* node, std::unordered_map<Node*, Node*> &parentMap);
};

#endif