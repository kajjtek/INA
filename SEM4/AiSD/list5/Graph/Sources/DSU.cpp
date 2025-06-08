#include "../Headers/DSU.h"

void DSU::make_set(Node* node){
    parentMap[node]= node;
    rankMap[node] = 0;
}

Node* DSU::find_set(Node* element) {
    if(parentMap[element]==element) return element; //root

    parentMap[element] = find_set(parentMap[element]);
    return parentMap[element];
}

void DSU::union_sets(Node* element1, Node* element2) {
    Node* root1 = find_set(element1);
    Node* root2 = find_set(element2);

    if(root1!=root2){
        if(rankMap[root1] < rankMap[root2]){
            std::swap(root1,root2);
        }
        parentMap[root2] = root1;
        if(rankMap[root1]==rankMap[root2]){
            rankMap[root1]++;
        }
    }
}