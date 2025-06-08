#include "../Headers/Node.h"

bool Node::operator==(const Node &node) const{
    return this->index==node.index;
}

void Node::addNeighbour(Node* node, double weight){
    neighbours[node] = weight;
}