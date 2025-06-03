#include "../Headers/RBTree.h"

RBNode* RBTree::NIL=nullptr;
long long RBTree::key_comparisons = 0;
long long RBTree::pointer_operations = 0;


int RBTree::height(){
    return calculateHeight(root);
}

void RBTree::insert(int key){
    pointer_operations++;
    if(root==NIL){
        root=new RBNode(key, NIL, Color::Black, NIL);
        pointer_operations += 4;
    } else doInsert(root, key);
    pointer_operations++;
    root->color=Color::Black;
}

void RBTree::doInsert(RBNode* current, int key){
    key_comparisons++;
    pointer_operations++;
    if(key>=current->value){
        key_comparisons++;
        pointer_operations++;
        if(current->right==NIL){
            current->right=new RBNode(key, current, Color::Red, NIL);
            pointer_operations += 5;
            recoloring(current->right);
        } 
        else doInsert(current->right,key);
        pointer_operations++;
    }else{
        key_comparisons++;
        pointer_operations++;
        if(current->left==NIL) {
            current->left=new RBNode(key, current, Color::Red, NIL);
            pointer_operations += 5;
            recoloring(current->left);
        }
        else doInsert(current->left,key);
        pointer_operations++;
    }
}

void RBTree::recoloring(RBNode* x){
    pointer_operations+=5;
    RBNode* parent = x->parent;
    if(parent==NIL) return;
    RBNode* grandfather = parent->parent;
    if(grandfather==NIL) return;
    if(parent->color!=Color::Black){
        pointer_operations+=5;
        key_comparisons++;
        RBNode* uncle = grandfather->left==parent?grandfather->right:grandfather->left;
        if(uncle->color==Color::Red){
            parent->color=Color::Black;
            uncle->color=Color::Black;
            grandfather->color=Color::Red;
            recoloring(grandfather);
            pointer_operations+=3;
        } else findRotation(x, parent, grandfather);
    }
}

void RBTree::findRotation(RBNode* x, RBNode* parent, RBNode* grandfather){
    pointer_operations+=10;
    if(parent==grandfather->left && x==parent->left) LLRotation(x, parent, grandfather);
    else if(parent==grandfather->left && x==parent->right) {LRRotation(x, parent, grandfather);}
    else if(parent==grandfather->right && x==parent->right) {RRRotation(x, parent, grandfather);}
    else if(parent==grandfather->right && x==parent->left) {RLRotation(x, parent, grandfather);}
    Color temp = grandfather->color;
    grandfather->color=parent->color;
    parent->color=temp;
}

void RBTree::fixDoubleBlack(RBNode* child, RBNode* parent){
    pointer_operations++;
    if(child==root){
        pointer_operations++;
        if(child!=NIL) {child->color=Color::Black;pointer_operations++;}
        return;
    }

    child->color=Color::DoubleBlack;
    RBNode* sibling = parent->left==child?parent->right:parent->left;
    pointer_operations+=2;
    if (isRed(sibling)) {
        parent->color = Color::Red;
        sibling->color = Color::Black;
        pointer_operations+=4;
        if (sibling == parent->left) {
            LLRotation(sibling->left, sibling, parent);
            sibling = parent->left;
        } else {
            RRRotation(sibling->right, sibling, parent); 
            sibling = parent->right;
        }
    }
    if(isBlack(sibling) && isBlack(sibling->left) && isBlack(sibling->right)){
        pointer_operations+=2;
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
            pointer_operations+=2;
            RBNode* nearChild = sibling->left;
            RBNode* farChild = sibling->right;
            if(isRed(farChild)){
                pointer_operations+=3;
                sibling->color=parent->color;
                farChild->color=Color::Black;
                RRRotation(farChild, sibling, parent);
                child->color=Color::Black;
                return;
            } else {
                pointer_operations+=7;
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
            pointer_operations+=2;
            RBNode* nearChild = sibling->right;
            RBNode* farChild = sibling->left;
            if(isRed(farChild)){
                pointer_operations+=3;
                sibling->color=parent->color;
                farChild->color=Color::Black;
                LLRotation(farChild, sibling, parent);
                child->color=Color::Black;
                return;
            } else {
                pointer_operations+=7;
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
    pointer_operations++;
    if(grandfather!=root){
        pointer_operations+=7;
        grandfather->parent->left==grandfather?grandfather->parent->left=parent:grandfather->parent->right=parent;
        parent->parent=grandfather->parent;
        grandfather->parent=parent;
        grandfather->left=parent->right;
        if(grandfather->left!=NIL) grandfather->left->parent=grandfather;
        parent->right=grandfather;
    } else {
        pointer_operations+=7;
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
        pointer_operations+=7;
        grandfather->parent->left==grandfather?grandfather->parent->left=parent:grandfather->parent->right=parent;
        parent->parent=grandfather->parent;
        grandfather->parent=parent;
        grandfather->right=parent->left;
        if(grandfather->right!=NIL) grandfather->right->parent=grandfather;
        parent->left=grandfather;
    } else {
        pointer_operations+=7;
        root=parent;
        parent->parent=NIL;
        grandfather->parent=parent;
        grandfather->right=parent->left;
        if(grandfather->right!=NIL) grandfather->right->parent=grandfather;
        parent->left=grandfather;
    }
}

void RBTree::RLRotation(RBNode* x, RBNode* parent, RBNode* grandfather){
    pointer_operations+=7;
    grandfather->right=x;
    x->parent=grandfather;
    parent->left=x->right;
    if(parent->left!=NIL) parent->left->parent=parent;
    x->right=parent;
    parent->parent=x;
    RRRotation(parent, parent->parent, parent->parent->parent);
}

RBNode* RBTree::findSuccessor(RBNode* current){
    pointer_operations++;
    if(current->right!=NIL){
        pointer_operations++;
        current=current->right;
        while(current->left!=NIL){
            current=current->left;
            pointer_operations++;
        }
        return current;
    } else {
        pointer_operations++;
        while(current->parent->left!=current){
            pointer_operations+=2;
            current=current->parent;
        }
        return current->parent;
    }
}

void RBTree::deletion(int key){
    pointer_operations++;
    doDeletion(root, key);
}

void RBTree::doDeletion(RBNode* current, int key){
    pointer_operations++;
    if(current==NIL){
        key_comparisons++;
        return;
    }
    key_comparisons++;
    pointer_operations += 2;
    if(key==this->root->value) {deletionRoot();return;}
    key_comparisons++;
    pointer_operations++;
    if(key>current->value) {
        pointer_operations++;
        doDeletion(current->right, key); 
    }   
    else {
        key_comparisons++;
        pointer_operations++;
        if(key<current->value) {
            pointer_operations++;
            doDeletion(current->left, key);
        }
        else {
            pointer_operations += 2;
            if(current->left==NIL && current->right==NIL){
                key_comparisons += 2;
                RBNode* child = NIL;
                pointer_operations++;
                key_comparisons++;
                pointer_operations += 4;
                current->parent->left==current?current->parent->left=child:current->parent->right=child;
                if(!isRed(current)){
                    key_comparisons++;
                    pointer_operations++;
                    fixDoubleBlack(child, current->parent);
                    pointer_operations += 2;
                }
                delete current;
                pointer_operations++;
            }
            else if(current->left!=NIL && current->right!=NIL){
                key_comparisons += 2;
                RBNode* successor = findSuccessor(current);
                pointer_operations += 2;
                current->value=successor->value;
                pointer_operations += 2;
                RBNode* successorChild = (successor->right != NIL) ? (key_comparisons++, successor->right) : (key_comparisons++, NIL);
                pointer_operations += 2;
                key_comparisons++;
                pointer_operations += 2;
                if (successor->parent->left == successor) {
                    successor->parent->left = successorChild;
                    pointer_operations += 2;
                } else {
                    successor->parent->right = successorChild;
                    pointer_operations += 2;
                }

                pointer_operations++;
                if(successorChild != NIL) {
                    key_comparisons++;
                    successorChild->parent = successor->parent;
                    pointer_operations += 2;
                    if(isRed(successor) || isRed(successorChild)){
                        key_comparisons += 2;
                        pointer_operations += 2;
                        successorChild->color=Color::Black;
                        pointer_operations++;
                    } else if(isBlack(successor) && isBlack(successorChild)){
                        key_comparisons += 2;
                        pointer_operations += 2;
                        fixDoubleBlack(successorChild, successorChild->parent);
                        pointer_operations += 2;
                    }
                } else {
                    key_comparisons++;
                    key_comparisons++;
                    pointer_operations += 4;
                    successor->parent->left==successor?successor->parent->left=successorChild:successor->parent->right=successorChild;
                    if(!isRed(successor)){
                        key_comparisons++;
                        pointer_operations++;
                        fixDoubleBlack(successorChild, successor->parent);
                        pointer_operations += 2;
                    }
                }
                delete successor;
                pointer_operations++;
            }
            else {
                key_comparisons += 2;
                RBNode* child = current->right!=NIL?(key_comparisons++, current->right):(key_comparisons++, current->left);
                pointer_operations += 2;
                child->parent=current->parent;
                pointer_operations += 2;
                key_comparisons++;
                pointer_operations += 4;
                current->parent->left==current?current->parent->left=child:current->parent->right=child;
                if(isRed(current) || isRed(child)){
                    key_comparisons += 2;
                    pointer_operations += 2;
                    child->color=Color::Black;
                    pointer_operations++;
                } else if(isBlack(current) && isBlack(child)){
                    key_comparisons += 2;
                    pointer_operations += 2;
                    fixDoubleBlack(child, child->parent);
                    pointer_operations += 2;
                }
                delete current;
                pointer_operations++;
            }
        }
    }
}

std::vector<RBNode*> RBTree::inOrderTraversal(){
    std::vector<RBNode*> result;
    pointer_operations++;
    if(root==NIL) {
        key_comparisons++;
        return result;
    }
    doInOrderTraversal(root, result);
    return result;
}

void RBTree::doInOrderTraversal(RBNode* current, std::vector<RBNode*> &result){
    pointer_operations++;
    if(current->left!=NIL) {
        key_comparisons++;
        doInOrderTraversal(current->left, result);
    }
    result.push_back(current);
    pointer_operations++;
    pointer_operations++;
    if(current->right!=NIL) {
        key_comparisons++;
        doInOrderTraversal(current->right, result);
    }
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
    pointer_operations++;
    if (root == NIL) {
        key_comparisons++;
        return;
    }

    Color original_deleted_color;
    RBNode* node_to_delete = root;
    pointer_operations++;
    RBNode* node_to_replace_root;
    RBNode* fixup_node = NIL;
    RBNode* fixup_parent = NIL;
    pointer_operations += 3;

    pointer_operations += 2;
    if (node_to_delete->left != NIL && node_to_delete->right != NIL) {
        key_comparisons += 2;
        RBNode* successor = findSuccessor(node_to_delete);
        pointer_operations += 2;
        
        node_to_delete->value = successor->value;
        pointer_operations += 2;

        original_deleted_color = successor->color;
        pointer_operations++;

        RBNode* successorChild = successor->right;
        pointer_operations += 2;
        
        fixup_node = successorChild;
        pointer_operations++;
        fixup_parent = successor->parent;
        pointer_operations += 2;

        key_comparisons++;
        pointer_operations += 2;
        if (successor->parent->left == successor) {
            successor->parent->left = successorChild;
            pointer_operations += 2;
        } else {
            successor->parent->right = successorChild;
            pointer_operations += 2;
        }

        pointer_operations++;
        if (successorChild != NIL) {
            key_comparisons++;
            successorChild->parent = successor->parent;
            pointer_operations += 2;
        }
        
        delete successor;
        pointer_operations++;

        key_comparisons++;
        if (original_deleted_color == Color::Black) {
            fixDoubleBlack(fixup_node, fixup_parent);
            pointer_operations += 2;
        }
        return;
    }

    pointer_operations += 2;
    if (node_to_delete->left == NIL && node_to_delete->right == NIL) {
        key_comparisons += 2;
        node_to_replace_root = NIL;
        pointer_operations++;
    } else {
        node_to_replace_root = (node_to_delete->left != NIL) ? (key_comparisons++, node_to_delete->left) : (key_comparisons++, node_to_delete->right);
        pointer_operations++;
    }

    original_deleted_color = node_to_delete->color;
    pointer_operations++;

    root = node_to_replace_root;
    pointer_operations++;
    
    pointer_operations++;
    if (root != NIL) {
        key_comparisons++;
        root->parent = NIL;
        pointer_operations += 2;
    }
    
    delete node_to_delete;
    pointer_operations++;

    key_comparisons++;
    if (original_deleted_color == Color::Black) {
        fixDoubleBlack(root, NIL); 
        pointer_operations += 2;
    }
}

int RBTree::calculateHeight(RBNode* current){
    pointer_operations++;
    if(current==NIL) {
        key_comparisons++;
        return -1;
    }

    int leftHeight = calculateHeight(current->left);
    int rightHeight = calculateHeight(current->right);
    pointer_operations += 2;
    return 1+std::max(leftHeight, rightHeight);
}