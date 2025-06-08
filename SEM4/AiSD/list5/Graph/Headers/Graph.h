#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_set>
#include "Node.h"
#include <set>
#include <tuple>
#include <cmath>

struct Comparator {
    bool operator()(const std::tuple<double, Node*, Node*> tuple1, std::tuple<double, Node*, Node*> tuple2) const {
        return std::get<0>(tuple1) < std::get<0>(tuple2);
    }
};

struct Comparator2 {
    bool operator()(const std::tuple<double, Node*, Node*> tuple1, std::tuple<double, Node*, Node*> tuple2) const {
        return std::get<0>(tuple1) > std::get<0>(tuple2);
    }
};

class Graph {
    public:
    std::unordered_set<Node*> allNodes;
    void clean(){
        for(Node* pointer:allNodes){
            delete pointer;
        }
        allNodes.clear();
    }
    void createRandomFullGraph(int n);
    Node* addNode(int i){
        Node *temp = new Node(i);
        allNodes.insert(temp);
        return temp;
    }
    Node* addNodeByNode(Node* temp){
        allNodes.insert(temp);
        return temp;
    }
    void addEdgeUndirected(Node* n1, Node* n2, double weight){
        n1->addNeighbour(n2, weight);
        n2->addNeighbour(n1, weight);
    }
    Graph primAlgorithm();
    ~Graph(){clean();}
    std::set<std::tuple<double, Node*, Node*>, Comparator> getAllEdges();
};

#endif