#include "../Headers/Tree.h"

Node* Tree::NIL=nullptr;
long long Tree::comparisons = 0;
long long Tree::pointer_operations = 0;

int Tree::height(){
    return calculateHeight(root);
}

void Tree::insert(int key){
    if(root==NIL){
        root=new Node(key, NIL, NIL);
        pointer_operations += 4;
    }else doInsert(root, key);
}

void Tree::doInsert(Node* current, int key){
    comparisons++;
    if(key>=current->value){
        pointer_operations++;
        if(current->right==NIL) {current->right=new Node(key, current, NIL); pointer_operations += 4;}
        else doInsert(current->right,key);
    }else{
        pointer_operations++;
        if(current->left==NIL) {current->left=new Node(key, current, NIL); pointer_operations += 4;}
        else doInsert(current->left,key);
    }
}

Node* Tree::findSuccessor(Node* current){
    pointer_operations++;
    if(current->right!=NIL){
        current=current->right;
        pointer_operations++;
        pointer_operations++;
        while(current->left!=NIL){
            comparisons++;
            current=current->left;
            pointer_operations+=2;
        }
        return current;
    } else {
        Node* ancestor = current->parent;
        pointer_operations++;
        comparisons++;
        pointer_operations++;
        comparisons++;
        while(ancestor != NIL && ancestor->left != current){
            current = ancestor;
            ancestor = ancestor->parent;
            pointer_operations+=4;
            comparisons+=2;
        }
        return ancestor;
    }
}

void Tree::deletion(int key){
    doDeletion(root, key);
}

void Tree::doDeletion(Node* current, int key){
    comparisons++;
    if(current==NIL) return;
    comparisons++;
    if(key==this->root->value) { deletionRoot();return;}
    comparisons++;
    if(key==current->value){
        // case 1 no children
        comparisons+=2;
        if(current->left==NIL && current->right==NIL){
            comparisons++;
            pointer_operations+=3;
            current->parent->left==current?current->parent->left=NIL:current->parent->right=NIL;
            delete current;
        }
        // case 3 two children
        else if(current->left!=NIL && current->right!=NIL){
            comparisons+=2;
            Node* successor = findSuccessor(current);
            pointer_operations+=2;
            current->value=successor->value;
            // doDeletion(successor, successor->value);
            comparisons+=2;
            pointer_operations+=4;
            Node* successorChild = (successor->right != NIL) ? successor->right : NIL;
            if (successor->parent->left == successor) {
                successor->parent->left = successorChild;
            } else {
                successor->parent->right = successorChild;
            }
            if(successorChild != NIL) {
                successorChild->parent = successor->parent;
                pointer_operations++;
            }
            delete successor;
        }
        // case 2 one children
        else {
            pointer_operations+=6;
            comparisons+=2;
            Node* child = current->right!=NIL?current->right:current->left;
            child->parent=current->parent;
            current->parent->left==current?current->parent->left=child:current->parent->right=child;
            delete current;
        }
    }else if(key<current->value) doDeletion(current->left, key);
    else doDeletion(current->right, key);
    comparisons++;
    pointer_operations++;
}

std::vector<Node*> Tree::inOrderTraversal(){
    std::vector<Node*> result;
    comparisons++;
    if(root==NIL) return result;
    doInOrderTraversal(root, result);
    return result;
}

void Tree::doInOrderTraversal(Node* current, std::vector<Node*> &result){
    comparisons++;
    if(current==NIL) return;
    doInOrderTraversal(current->left, result);
    result.push_back(current);
    doInOrderTraversal(current->right, result);
    pointer_operations+=2;
}

 void Tree::print_BST(){
    std::vector<bool> leftTrace(height() + 2, false); // +2 for safety, height() can be -1 for empty tree
    std::vector<bool> rightTrace(height() + 2, false);
    doPrint(root, 0, '-', leftTrace, rightTrace);
}

void Tree::doPrint(Node* current, int depth, char prefix,
                std::vector<bool>& leftTrace, std::vector<bool>& rightTrace) {
    if (current == NIL) return;

    // Print left subtree first (recursive call)
    if (current->left != NIL) {
        doPrint(current->left, depth + 1, '/', leftTrace, rightTrace);
    }

    // Adjust traces based on the current node's parent's prefix
    if (prefix == '/') leftTrace[depth - 1] = true; // 'true' means draw '|'
    if (prefix == '\\') rightTrace[depth - 1] = false; // 'false' means draw ' '

    // Print leading spaces and vertical lines
    if (depth == 0) {
        std::cout << "-";
    } else {
        std::cout << " ";
    }

    for (int i = 0; i < depth - 1; i++) {
        if (leftTrace[i] || rightTrace[i]) {
            std::cout << "| ";
        } else {
            std::cout << "  ";
        }
    }

    // Print prefix and node value
    if (depth > 0) {
        std::cout << prefix << "-";
    }
    std::cout << "[" << current->value << "]\n"; // Changed from current->element to current->value

    // Reset the current depth's left trace, as this branch has ended
    leftTrace[depth] = false;

    // Print right subtree (recursive call)
    if (current->right != NIL) {
        rightTrace[depth] = true; // Mark that a right child exists at this depth
        doPrint(current->right, depth + 1, '\\', leftTrace, rightTrace);
    }
}

void Tree::clean(){
    doClean(root);
}

void Tree::doClean(Node* current){
    if(current==NIL) return;
    doClean(current->left);
    doClean(current->right);
    delete current;
}

void Tree::deletionRoot(){
    comparisons+=3;
    if(root==NIL){return;}
    if(root->left==NIL && root->right==NIL){
        delete root;
        root=NIL;
        pointer_operations++;
    }
    // case 3 two children
    else if(root->left!=NIL && root->right!=NIL){
        comparisons+=4;
        pointer_operations+=5;
        Node* successor = findSuccessor(root);
        root->value=successor->value;
        // doDeletion(successor, successor->value);
        // doDeletion(successor, successor->value);
        Node* successorChild = (successor->right != NIL) ? successor->right : NIL;
        if (successor->parent->left == successor) {
            successor->parent->left = successorChild;
        } else {
            successor->parent->right = successorChild;
        }
        if(successorChild != NIL) {
            successorChild->parent = successor->parent;
            pointer_operations++;
        }
        delete successor;
    }
    // case 2 one children
    else {
        Node* child = root->right!=NIL?root->right:root->left;
        child->parent=NIL;
        delete root;
        this->root=child;
        pointer_operations += 4;
        comparisons++;
    }
}

int Tree::calculateHeight(Node* current){
    comparisons++;
    if(current==NIL) return -1;

    pointer_operations+=2;
    int leftHeight = calculateHeight(current->left);
    int rightHeight = calculateHeight(current->right);
    return 1+std::max(leftHeight, rightHeight);
}