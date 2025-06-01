#include "../Headers/RBTree.h"

RBNode* RBTree::NIL=nullptr;

int RBTree::height(){
    return calculateHeight(root);
}

void RBTree::insert(int key){
    if(root==NIL){
        root=new RBNode(key, NIL, Color::Black, NIL);
    } else doInsert(root, key);
    root->color=Color::Black;
}

void RBTree::doInsert(RBNode* current, int key){
    if(key>=current->value){
        if(current->right==NIL){
            current->right=new RBNode(key, current, Color::Red, NIL);
            recoloring(current->right);
        } 
        else doInsert(current->right,key);
    }else{
        if(current->left==NIL) {
            current->left=new RBNode(key, current, Color::Red, NIL);
            recoloring(current->left);
        }
        else doInsert(current->left,key);
    }
}

void RBTree::recoloring(RBNode* x){
    RBNode* parent = x->parent;
    if(parent==NIL) return;
    RBNode* grandfather = parent->parent;
    if(grandfather==NIL) return;
    if(parent->color!=Color::Black){
        RBNode* uncle = grandfather->left==parent?grandfather->right:grandfather->left;
        if(uncle->color==Color::Red){
            parent->color=Color::Black;
            uncle->color=Color::Black;
            grandfather->color=Color::Red;
            recoloring(grandfather);
        } else findRotation(x, parent, grandfather);
    }
}

void RBTree::findRotation(RBNode* x, RBNode* parent, RBNode* grandfather){
    if(parent==grandfather->left && x==parent->left) LLRotation(x, parent, grandfather);
    else if(parent==grandfather->left && x==parent->right) LRRotation(x, parent, grandfather);
    else if(parent==grandfather->right && x==parent->right) RRRotation(x, parent, grandfather);
    else if(parent==grandfather->right && x==parent->left) RLRotation(x, parent, grandfather);
    Color temp = grandfather->color;
    grandfather->color=parent->color;
    parent->color=temp;
}

void RBTree::fixDoubleBlack(RBNode* child, RBNode* parent){

    if(child==root){
        if(child!=NIL) child->color=Color::Black;
        return;
    }

    child->color=Color::DoubleBlack;
    RBNode* sibling = parent->left==child?parent->right:child->parent->left;

    if(isRed(sibling)){
        sibling->color=Color::Black;
        parent->color=Color::Red;
        if(sibling==parent->left) LLRotation(sibling->left, sibling, parent);
        else RRRotation(sibling->right, sibling, parent);
        child->color=Color::Black;
        return;
    }
    else if(isBlack(sibling) && isBlack(sibling->left) && isBlack(sibling->right)){
        sibling->color=Color::Red;
        if(isRed(parent)) parent->color=Color::Black;
        else {
            child->color=Color::Black;
            parent->color=Color::DoubleBlack;
            fixDoubleBlack(parent, parent->parent);
        }
    }
    else
    {
        if(child==parent->left){
            RBNode* nearChild = sibling->left;
            RBNode* farChild = sibling->right;
            if(isRed(farChild)){
                sibling->color=parent->color;
                farChild->color=Color::Black;
                RRRotation(farChild, sibling, parent);
                child->color=Color::Black;
                return;
            } else {
                nearChild->color=Color::Black;
                sibling->color=Color::Red;
                LLRotation(nearChild->left, nearChild, sibling);
                sibling=nearChild;
                farChild=sibling->right;
                sibling->color=parent->color;
                parent->color=Color::Black;
                farChild->color=Color::Black;
                RRRotation(farChild, sibling, parent);
                child->color=Color::Black;
                return;
            }
        }else{
            RBNode* nearChild = sibling->right;
            RBNode* farChild = sibling->left;
            if(isRed(farChild)){
                sibling->color=parent->color;
                farChild->color=Color::Black;
                LLRotation(farChild, sibling, parent);
                child->color=Color::Black;
                return;
            } else {
                nearChild->color=Color::Black;
                sibling->color=Color::Red;
                RRRotation(nearChild->right, nearChild, sibling);
                sibling=nearChild;
                farChild=sibling->left;
                sibling->color=parent->color;
                parent->color=Color::Black;
                farChild->color=Color::Black;
                LLRotation(farChild, sibling, parent);
                child->color=Color::Black;
                return;
            }
        }
    } 
}

void RBTree::LLRotation(RBNode* x, RBNode* parent, RBNode* grandfather){
    if(grandfather!=root){
        grandfather->parent->left==grandfather?grandfather->parent->left=parent:grandfather->parent->right=parent;
        parent->parent=grandfather->parent;
        grandfather->parent=parent;
        grandfather->left=parent->right;
        if(grandfather->left!=NIL) grandfather->left->parent=grandfather;
        parent->right=grandfather;
    } else {
        root=parent;
        parent->parent=NIL;
        grandfather->parent=parent;
        grandfather->left=parent->right;
        if(grandfather->left!=NIL) grandfather->left->parent=grandfather;
        parent->right=grandfather;
    } 
}

void RBTree::LRRotation(RBNode* x, RBNode* parent, RBNode* grandfather){
    // grandfather->left=x;
    // x->parent=grandfather;
    // parent->right=x->left;
    // if(parent->right!=NIL) parent->right->parent=parent;
    // x->left=parent;
    // parent->parent=x;
    // LLRotation(parent, parent->parent, parent->parent->parent);
    RRRotation(x->right, x, parent);
    LLRotation(parent, x, grandfather);
}

void RBTree::RRRotation(RBNode* x, RBNode* parent, RBNode* grandfather){
    if(grandfather!=root){
        grandfather->parent->left==grandfather?grandfather->parent->left=parent:grandfather->parent->right=parent;
        parent->parent=grandfather->parent;
        grandfather->parent=parent;
        grandfather->right=parent->left;
        if(grandfather->right!=NIL) grandfather->right->parent=grandfather;
        parent->left=grandfather;
    } else {
        root=parent;
        parent->parent=NIL;
        grandfather->parent=parent;
        grandfather->right=parent->left;
        if(grandfather->right!=NIL) grandfather->right->parent=grandfather;
        parent->left=grandfather;
    }
}

void RBTree::RLRotation(RBNode* x, RBNode* parent, RBNode* grandfather){
    grandfather->right=x;
    x->parent=grandfather;
    parent->left=x->right;
    if(parent->left!=NIL) parent->left->parent=parent;
    x->right=parent;
    parent->parent=x;
    RRRotation(parent, parent->parent, parent->parent->parent);
}

RBNode* RBTree::findSuccessor(RBNode* current){
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

void RBTree::deletion(int key){
    doDeletion(root, key);
}

void RBTree::doDeletion(RBNode* current, int key){
    if(current==NIL)return;
    if(key==this->root->value) {deletionRoot();return;}
    if(key>current->value)  doDeletion(current->right, key);    
    else if(key<current->value) doDeletion(current->left, key);
    else {
        // case 1 no children
        if(current->left==NIL && current->right==NIL){
            RBNode* child = NIL;
            current->parent->left==current?current->parent->left=child:current->parent->right=child;
            if(!isRed(current)){
                fixDoubleBlack(child, current->parent);
            }
            delete current;
        }
        // case 3 two children
        else if(current->left!=NIL && current->right!=NIL){
            RBNode* successor = findSuccessor(current);
            current->value=successor->value;
            RBNode* successorChild = (successor->right != NIL) ? successor->right : NIL;
            if (successor->parent->left == successor) {
                successor->parent->left = successorChild;
            } else {
                successor->parent->right = successorChild;
            }

            if(successorChild != NIL) { //one children
                successorChild->parent = successor->parent;
                if(isRed(successor) || isRed(successorChild)){
                successorChild->color=Color::Black;
                } else if(isBlack(successor) && isBlack(successorChild)){
                    fixDoubleBlack(successorChild, successorChild->parent);
                }
            } else { //no children
                successor->parent->left==successor?successor->parent->left=successorChild:successor->parent->right=successorChild;
                if(!isRed(successor)){
                    fixDoubleBlack(successorChild, successor->parent);
                }
            }
            delete successor;
        }
        // case 2 one children
        else {
            RBNode* child = current->right!=NIL?current->right:current->left;
            child->parent=current->parent;
            current->parent->left==current?current->parent->left=child:current->parent->right=child;
            if(isRed(current) || isRed(child)){
                child->color=Color::Black;
            } else if(isBlack(current) && isBlack(child)){
                fixDoubleBlack(child, child->parent);
            }
            delete current;
        }
    }
}

std::vector<RBNode*> RBTree::inOrderTraversal(){
    std::vector<RBNode*> result;
    if(root==NIL) return result;
    doInOrderTraversal(root, result);
    return result;
}

void RBTree::doInOrderTraversal(RBNode* current, std::vector<RBNode*> &result){
    if(current->left!=NIL) doInOrderTraversal(current->left, result);
    result.push_back(current);
    if(current->right!=NIL) doInOrderTraversal(current->right, result);
}

 void RBTree::print_BST(){
    std::vector<bool> leftTrace(height() + 2, false); // +2 for safety, height() can be -1 for empty RBTree
    std::vector<bool> rightTrace(height() + 2, false);
    doPrint(root, 0, '-', leftTrace, rightTrace);
}

void RBTree::doPrint(RBNode* current, int depth, char prefix,
                std::vector<bool>& leftTrace, std::vector<bool>& rightTrace) {
    if (current == NIL) return;

    // Print left subRBTree first (recursive call)
    if (current->left != NIL) {
        doPrint(current->left, depth + 1, '/', leftTrace, rightTrace);
    }

    // Adjust traces based on the current RBNode's parent's prefix
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

    // Print prefix and RBNode value
    if (depth > 0) {
        std::cout << prefix << "-";
    }
    std::cout << "[" << current->value;
    if(isBlack(current)) std::cout<<"B"<< "]\n";
    else std::cout<<"R"<< "]\n"; // Changed from current->element to current->value

    // Reset the current depth's left trace, as this branch has ended
    leftTrace[depth] = false;

    // Print right subRBTree (recursive call)
    if (current->right != NIL) {
        rightTrace[depth] = true; // Mark that a right child exists at this depth
        doPrint(current->right, depth + 1, '\\', leftTrace, rightTrace);
    }
}

void RBTree::clean(){
    doClean(root);
}

void RBTree::doClean(RBNode* current){
    if(current==NIL) return;
    doClean(current->left);
    doClean(current->right);
    delete current;
}

void RBTree::deletionRoot() {
    if (root == NIL) {
        return;
    }

    RBNode* node_to_delete = root;
    RBNode* node_to_replace_root;
    Color original_deleted_color;
    RBNode* fixup_node = NIL;
    RBNode* fixup_parent = NIL;

    if (node_to_delete->left != NIL && node_to_delete->right != NIL) {
        RBNode* successor = findSuccessor(node_to_delete);
        
        node_to_delete->value = successor->value;

        original_deleted_color = successor->color;

        RBNode* successorChild = successor->right;
        
        fixup_node = successorChild;
        fixup_parent = successor->parent;

        if (successor->parent->left == successor) {
            successor->parent->left = successorChild;
        } else {
            successor->parent->right = successorChild;
        }

        if (successorChild != NIL) {
            successorChild->parent = successor->parent;
        }
        
        delete successor;

        if (original_deleted_color == Color::Black) {
            fixDoubleBlack(fixup_node, fixup_parent);
        }
        return;
    }

    if (node_to_delete->left == NIL && node_to_delete->right == NIL) {
        node_to_replace_root = NIL;
    } else {
        node_to_replace_root = (node_to_delete->left != NIL) ? node_to_delete->left : node_to_delete->right;
    }

    original_deleted_color = node_to_delete->color;

    root = node_to_replace_root;
    
    if (root != NIL) {
        root->parent = NIL;
    }
    
    delete node_to_delete;

    if (original_deleted_color == Color::Black) {
        fixDoubleBlack(root, NIL); 
    }
}

int RBTree::calculateHeight(RBNode* current){
    if(current==NIL) return -1;

    int leftHeight = calculateHeight(current->left);
    int rightHeight = calculateHeight(current->right);
    return 1+std::max(leftHeight, rightHeight);
}