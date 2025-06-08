#include "../Headers/GreedyAlgorithms.h"
#include <queue>
#include <tuple>
#include <vector>
#include <map>


// struct Comparator {
//     bool operator()(const std::tuple<double, Node*, Node*> tuple1, std::tuple<double, Node*, Node*> tuple2){
//         return std::get<0>(tuple1) > std::get<0>(tuple2);
//     }
// };

Graph GreedyAlgorithms::prism(Graph &g){ //queue weight, parent, the node
    Graph MST;
    std::unordered_set<Node*> visited;
    std::priority_queue<std::tuple<double, Node*, Node*>, std::vector<std::tuple<double, Node*, Node*>>, Comparator2> queue;
    Node* start = *g.allNodes.begin();
    queue.push({0, nullptr, start});
    Node* current;
    double weight;
    Node* parent;
    while(!queue.empty() && MST.allNodes.size() < g.allNodes.size()){
        do{
            std::tuple<double, Node*, Node*> tuple = queue.top();
            queue.pop();
            weight = std::get<0>(tuple);
            parent = std::get<1>(tuple);
            current = std::get<2>(tuple);
        } while(visited.contains(current));
        Node* temp = MST.addNode(current->index);
        visited.insert(current);
        if(parent!=nullptr) MST.addEdgeUndirected(temp, parent, weight);
        for(auto i=current->neighbours.begin(); i!=current->neighbours.end(); ++i){
            Node* potentialNode = i->first;
            double w = i->second;
            queue.push({w, temp, potentialNode});
        }
    }
    return MST;
}

Graph GreedyAlgorithms::Kruskal(Graph &g){
    Graph MST;
    std::unordered_map<Node*, Node*> cloneMap;
    for(Node* node: g.allNodes){
        cloneMap[node] = nullptr;//we keep elements of g in dsu map thus we will control it using visited
    }
    std::set<std::tuple<double, Node*, Node*>, Comparator> allEdges = g.getAllEdges();
    DSU dsu;
    for(Node* node: g.allNodes){
        dsu.make_set(node);//we keep elements of g in dsu map thus we will control it using visited
    }
    int edgecounter = 0;
    auto it = allEdges.begin();
    while(it!=allEdges.end() && MST.allNodes.size()<g.allNodes.size()){
        std::tuple<double, Node*, Node*> edge = *it;
        double w = std::get<0>(edge);
        Node* node1 = std::get<1>(edge);
        Node* node2 = std::get<2>(edge);
        if(dsu.find_set(node1)!=dsu.find_set(node2)){
            if(cloneMap[node1]==nullptr) {cloneMap[node1]=new Node(node1->index); MST.addNodeByNode(cloneMap[node1]);}
            if(cloneMap[node2]==nullptr) {cloneMap[node2]=new Node(node2->index); MST.addNodeByNode(cloneMap[node2]);}
            Node* temp1=cloneMap[node1];
            Node* temp2=cloneMap[node2];
            MST.addEdgeUndirected(temp1, temp2, w);
            edgecounter++;
        }
        it++;
    }
    return MST;
}

void GreedyAlgorithms::informationPassing(Graph &g){
    std::unordered_map<Node*, int> sizeMap;
    std::unordered_map<Node*, std::vector<Node*>> optimalChildOrder;

    calculateSize(sizeMap, *g.allNodes.begin());

    for(Node* node : g.allNodes){
        std::vector<Node*> order;
        for(auto it=node->neighbours.begin(); it!=node->neighbours.end(); it++){
            Node* neighbour = it->first;
            order.push_back(neighbour);
        }

        std::sort(order.begin(), order.end(), [&sizeMap](Node* a, Node* b) {
            return sizeMap[a] > sizeMap[b];
        });
        optimalChildOrder[node] = order;
    }



}

int calculateSize(std::unordered_map<Node*, int> &map, Node* node){
    int size = 1;
    for(auto it=node->neighbours.begin(); it!=node->neighbours.end(); it++){
        Node* neighbour = it->first;
        size+=calculateSize(map , neighbour);
    }
    map[node]=size;

    return size;
}

// void GreedyAlgorithms::setupSizeMap(std::unordered_map<Node*, int> &map, Node* root){
//     int size = 1;
//     for(Node* neigbour: root->neighbours)
// }