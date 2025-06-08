#include "../Headers/Graph.h"
#include <random>


void Graph::createRandomFullGraph(int n){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0,1);
    clean();
    for(int i=1; i<=n;i++){
        addNode(i);
    }
    for(auto i=allNodes.begin(); i!=allNodes.end(); i++){
        auto j = i;
        ++j;
        for(;j!=allNodes.end(); j++){
            Node *n1 = *i;
            Node *n2 = *j;
            addEdgeUndirected(n1, n2, dist(gen));
        }
    }
}

std::set<std::tuple<double, Node*, Node*>, Comparator> Graph::getAllEdges(){
    std::set<std::tuple<double, Node*, Node*>, Comparator> set;
    for(auto it = this->allNodes.begin(); it!=this->allNodes.end(); ++it){
        Node* node = *it;
        for(auto jt = node->neighbours.begin(); jt!=node->neighbours.end(); ++jt){
            Node* neighbour = jt->first;
            double w = jt->second;
            std::min(node->index, neighbour->index)==node->index?set.insert({w, node, neighbour}):set.insert({w, neighbour, node});
        }
    }
    return set;
};