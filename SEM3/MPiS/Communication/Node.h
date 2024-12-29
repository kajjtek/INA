#include <random>

class Node
{
    public:
    bool hasMessage = false;
    bool receiveMessage(double random_number);
    Node(double x);
    double probability;
};

Node::Node(double x){
    this->probability=x;
}

bool Node::receiveMessage(double random_number){
    if(!this->hasMessage){
        if(random_number<=this->probability){
            this->hasMessage=true;
            return true;
        }else{
            return false;
        }
    }
    return false;
}