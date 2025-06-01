#include "../Headers/STree.h"

SNode* STree::NIL=nullptr;

int STree::height(){
    return calculateHeight(root);
}

void STree::insert(int key){
    if(root==NIL){
        root=new SNode(key, NIL, NIL);
    } else doInsert(root, key);
}

void STree::Zig(SNode* x){
    if(x==NIL) return;
    SNode* parent = x->parent;
    parent->left=x->right;
    if(parent->left!=NIL) parent->left->parent=parent;
    x->right=parent;
    parent->parent=x;
}

void STree::Zag(SNode* x){
    if(x==NIL) return;
    SNode* parent = x->parent;
    parent->right=x->left;
    if(parent->right!=NIL) parent->right->parent=parent;
    x->left=parent;
    parent->parent=x;    
}

void STree::ZigZig(SNode* x){//where x is the one we want to up twice 
    if(x==NIL) return;
    Zig(x->parent);
    Zig(x);
}

void STree::ZagZag(SNode* x){//where x is the one we want to up twice 
    if(x==NIL) return;
    Zag(x->parent);
    Zag(x);
}

void STree::ZigZag(SNode* x){//where x is the one we want to up twice 
    if(x==NIL) return;
    Zag(x);
    Zig(x);
}

void STree::ZagZig(SNode* x){//where x is the one we want to up twice 
    if(x==NIL) return;
    Zig(x);
    Zag(x);
}

void STree::Splay(SNode* x){
    while(x!=root){
        if(x->parent==root){
            if(x==x->parent->left) {
                Zig(x);
            }else Zag(x);
        } else {
            SNode* parent = x->parent;
            SNode* grandfather = parent->parent;
            if(x==parent->left && parent==grandfather->left) ZigZig(x);
            else if(x==parent->right && parent==grandfather->right) ZagZag(x);
            else if(x==parent->right && parent==grandfather->left) ZigZag(x);
            else ZagZig(x);
        }
    }
    root=x;
}

void STree::SplayFind(int key){
    SNode* x = find(key);
    while(x!=root){
        if(x->parent==root){
            if(x==x->parent->left) {
                Zig(x);
            }else Zag(x);
        } else {
            SNode* parent = x->parent;
            SNode* grandfather = parent->parent;
            if(x==parent->left && parent==grandfather->left) ZigZig(x);
            else if(x==parent->right && parent==grandfather->right) ZagZag(x);
            else if(x==parent->right && parent==grandfather->left) ZigZag(x);
            else ZagZig(x);
        }
    }
    root=x;
}

void STree::doInsert(SNode* current, int key){
    if(key>=current->value){
        if(current->right==NIL){
            current->right=new SNode(key, current, NIL);
            Splay(current->right);
        }
        else doInsert(current->right,key);
    }else{
        if(current->left==NIL) {current->left=new SNode(key, current, NIL);Splay(current->left);}
        else doInsert(current->left,key);
    }
}

SNode* STree::findSuccessor(SNode* current){
    if(current->right!=NIL){
        current=current->right;
        while(current->left!=NIL){
            current=current->left;
        }
        return current;
    } else {
        while(current->parent->left!=current){
            current=current->parent;
        }
        return current->parent;
    }
}

void STree::deletion(int key) {
    if(root == NIL){
        return;
    }

    SplayFind(key); 

    if(root->value != key){
        return;
    }

    SNode* nodeToDelete = root;
    SNode* leftSubtree = nodeToDelete->left;
    SNode* rightSubtree = nodeToDelete->right;

    nodeToDelete->left = NIL;
    nodeToDelete->right = NIL;

    delete nodeToDelete; 

    if (leftSubtree == NIL) {
        root = rightSubtree;
        if (root != NIL) {
            root->parent = NIL;
        }
    } else if (rightSubtree == NIL) {
        root = leftSubtree;
        if (root != NIL) {
            root->parent = NIL;
        }
    } else {
        root = leftSubtree;
        root->parent = NIL;
        
        SNode* current = root;
        while (current->right != NIL) {
            current = current->right;
        }
        
        Splay(current); 

        root->right = rightSubtree;
        if (rightSubtree != NIL) {
            rightSubtree->parent = root;
        }
    }
}

SNode* STree::findMinimum(SNode* current){
    while(current->left!=NIL){
        current=current->left;
    }
    return current;
}

void STree::doDeletion(SNode* current, int key){
    if(key==this->root->value) deletionRoot();
    if(key==current->value){
        // case 1 no children
        if(current->left==NIL && current->right==NIL){
            current->parent->left==current?current->parent->left=NIL:current->parent->right=NIL;
            delete current;
        }
        // case 3 two children
        else if(current->left!=NIL && current->right!=NIL){
            SNode* successor = findSuccessor(current);
            current->value=successor->value;
            doDeletion(successor, successor->value);
        }
        // case 2 one children
        else {
            SNode* child = current->right!=NIL?current->right:current->left;
            child->parent=current->parent;
            current->parent->left==current?current->parent->left=child:current->parent->right=child;
            delete current;
        }
    }else if(key<current->value) doDeletion(current->left, key);
    else doDeletion(current->right, key);
}

std::vector<SNode*> STree::inOrderTraversal(){
    std::vector<SNode*> result;
    if(root==NIL) return result;
    doInOrderTraversal(root, result);
    return result;
}

SNode* STree::find(int key){
    SNode* current = root;
    while(current!=NIL && current->value!=key){
        if(key>=current->value) current=current->right;
        else current=current->left;
    }
    return current;
}

void STree::doInOrderTraversal(SNode* current, std::vector<SNode*> &result){
    if(current->left!=NIL) doInOrderTraversal(current->left, result);
    result.push_back(current);
    if(current->right!=NIL) doInOrderTraversal(current->right, result);
}

 void STree::print_BST(){
    std::vector<bool> leftTrace(height() + 2, false); // +2 for safety, height() can be -1 for empty STree
    std::vector<bool> rightTrace(height() + 2, false);
    doPrint(root, 0, '-', leftTrace, rightTrace);
}

void STree::doPrint(SNode* current, int depth, char prefix,
                std::vector<bool>& leftTrace, std::vector<bool>& rightTrace) {
    if (current == NIL) return;

    // Print left subSTree first (recursive call)
    if (current->left != NIL) {
        doPrint(current->left, depth + 1, '/', leftTrace, rightTrace);
    }

    // Adjust traces based on the current SNode's parent's prefix
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

    // Print prefix and SNode value
    if (depth > 0) {
        std::cout << prefix << "-";
    }
    std::cout << "[" << current->value << "]\n"; // Changed from current->element to current->value

    // Reset the current depth's left trace, as this branch has ended
    leftTrace[depth] = false;

    // Print right subSTree (recursive call)
    if (current->right != NIL) {
        rightTrace[depth] = true; // Mark that a right child exists at this depth
        doPrint(current->right, depth + 1, '\\', leftTrace, rightTrace);
    }
}

void STree::clean(){
    doClean(root);
}

void STree::doClean(SNode* current){
    if(current==NIL) return;
    doClean(current->left);
    doClean(current->right);
    delete current;
}

void STree::deletionRoot(){
    if(root==NIL){return;}
    if(root->left==NIL && root->right==NIL){
        delete root;
        root=NIL;
    }
    // case 3 two children
    else if(root->left!=NIL && root->right!=NIL){
        SNode* successor = findSuccessor(root);
        root->value=successor->value;
        doDeletion(successor, successor->value);
    }
    // case 2 one children
    else {
        SNode* child = root->right!=NIL?root->right:root->left;
        child->parent=NIL;
        delete root;
        this->root=child;
    }
}

int STree::calculateHeight(SNode* current){
    if(current==NIL) return -1;

    int leftHeight = calculateHeight(current->left);
    int rightHeight = calculateHeight(current->right);
    return 1+std::max(leftHeight, rightHeight);
}