#include "../Headers/GreedyAlgorithms.h"
#include <queue>
#include <tuple>
#include <vector>
#include <map>
#include <iostream>


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

void parentEstablisher(std::unordered_map<Node*,Node*> &parentMap, Node* node,Node* parent){
    parentMap[node] = parent;

    for(auto it=node->neighbours.begin(); it!=node->neighbours.end(); it++){
        Node* neighbour = it->first;
        if(parentMap.contains(neighbour) || parent==neighbour) continue;
        parentEstablisher(parentMap, neighbour, node);
    }
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

int GreedyAlgorithms::calculateSize(std::unordered_map<Node*, int> &map, Node* node, std::unordered_map<Node*, Node*> &parentMap) {
    int size = 1;
    for(auto it=node->neighbours.begin(); it!=node->neighbours.end(); it++){
        Node* neighbour = it->first;
        if(neighbour==parentMap[node]) continue;
        size+=calculateSize(map , neighbour, parentMap);
    }
    map[node]=size;

    return size;
}

int GreedyAlgorithms::informationPassing(Graph &g){
    std::unordered_map<Node*, int> sizeMap;
    std::unordered_map<Node*, std::vector<Node*>> optimalChildOrder;
    std::unordered_map<Node*, Node*> parentMap;

    Node* start = *g.allNodes.begin();

    parentEstablisher(parentMap, start, nullptr);

    calculateSize(sizeMap, *g.allNodes.begin(), parentMap);

    for(Node* node : g.allNodes){
        std::vector<Node*> order;
        for(auto it=node->neighbours.begin(); it!=node->neighbours.end(); it++){
            Node* neighbour = it->first;
            if(neighbour==parentMap[node]) continue;
            order.push_back(neighbour);
        }

        std::sort(order.begin(), order.end(), [&sizeMap](Node* a, Node* b) {
            return sizeMap[a] > sizeMap[b];
        });
        optimalChildOrder[node] = order;
    }

    int time = 0;
    std::unordered_map<int, std::vector<Node*>> timeArrival;
    std::vector<Node*> currentRound;
    timeArrival[0] = {start};
    do
    {
        currentRound = timeArrival[time];
        for(Node* currentNode: currentRound){
            std::vector<Node*> order = optimalChildOrder[currentNode];
            for(int i=0; i<order.size(); i++){
                timeArrival[time+i+1].push_back(order.at(i));
            }
        }
        time++;
    } while (!currentRound.empty());

    return time-1;

    // for(int k=0; k<timeArrival.size(); k++){
    //     std::vector<Node*> currentRound = timeArrival[k];
    //     if(currentRound.empty())continue;
    //     std::cout<<"ROUND "<<k<<std::endl;
    //     for(Node* currentNode: currentRound){
    //         std::cout<<currentNode->index<<", ";
    //     }
    //     std::cout << std::endl;
    // }
}



// void GreedyAlgorithms::setupSizeMap(std::unordered_map<Node*, int> &map, Node* root){
//     int size = 1;
//     for(Node* neigbour: root->neighbours)
// }