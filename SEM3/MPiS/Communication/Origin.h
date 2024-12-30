#include <vector>
#include "Node.h"
#include <random>

using namespace std;
class Origin
{
    private:
    vector<Node> children;
    double probability;
    int gotMessage = 0;
    public:
    int findTrend();
    int execute();
    void setProbability(double x);
    void resetCounter();
    void resetChildren();
    void sendMessage(Node& node, double random);
    void createChildren(int n);//creates as many children as the user whishes
};

int Origin::findTrend(){
    random_device rd;
    mt19937 generator(rd()); 
    uniform_real_distribution<> dis(0.0, 1.0);
    int tura = 0;
    while(!children.at(99).hasMessage){
        double random_number = dis(generator);
        sendMessage(children.at(99), random_number);
        tura++;
    }
    return tura;
}


int Origin::execute(){
    random_device rd;
    mt19937 generator(rd()); 
    uniform_real_distribution<> dis(0.0, 1.0);
    int tura = 0;
    while(gotMessage<children.size()){
        for (int i = 0; i < children.size(); i++)//iterujemy po dzieciach i wysylamy kazdemu wiadomosc
        {
        double random_number = dis(generator);
        sendMessage(children.at(i), random_number);
        }
        tura++;
    }
    return tura;
}

void Origin::resetChildren(){
    for (int i = 0; i < children.size(); i++)
    {
        children.at(i).hasMessage=false;
    }
}

void Origin::setProbability(double x){
    this->probability=x;
}

void Origin::resetCounter(){
    this->gotMessage=0;
}

void Origin::createChildren(int n){
    for (int i = children.size(); i < n; i++)
    {
        children.push_back(Node(this->probability));
    }
}

void Origin::sendMessage(Node& node, double random){
    if(node.receiveMessage(random)){
        this->gotMessage++;
    }
}