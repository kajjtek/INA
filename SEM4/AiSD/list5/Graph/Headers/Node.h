#ifndef NODE_H
#define NODE_H
#include <functional>
#include <map>


class Edge;

class Node {
    public:
    std::map<Node*, double> neighbours;
    int index;
    Node(int x): index(x) {};
    bool operator==(const Node &node) const;
    void addNeighbour(Node* node, double weight);
    ~Node(){
        neighbours.clear();
    }
};

namespace std {
    template <>
    struct hash<Node> {
        size_t operator()(const Node& n) const {
            return std::hash<int>()(n.index);
        }
    };
}

#endif