#include <iostream>

template <class T>
struct Node {
    private:
    T value;
    Node *next;
    void setValue(T input){
        this->value=input;
    }
    public:
    Node* getNext(){
        return this->next;
    }
    T getValue(){
        return this->value;
    }
    void setNext(Node<T>* next){
        this->next=next;
    }
    Node(T input){
        setValue(input);
        setNext(nullptr);
    }
};

template <class T>
class lifo {
    private:
    Node<T> *rear;
    public:
    lifo():rear(nullptr){}
    void add(T value){
        if(rear==nullptr){
            Node<T>* addedNode = new Node<T>(value);
            rear=addedNode;
        }else{
            Node<T>* addedNode = new Node<T>(value);
            addedNode->setNext(rear);
            rear=addedNode;
        }
    }
    T get(){
        if(rear==nullptr){
            throw std::runtime_error("queue is empty");
        }
        else{
            Node<T> *temp = rear;
            rear = rear->getNext();
            return temp->getValue();
        }
    }
    ~lifo(){
        while(!(rear==nullptr)){
            get();
        }
    }
};

int main(){
    lifo<int> myqueue;
    for(int i=0; i<50;i++){
        myqueue.add(i);
        std::cout << "Input: " << i <<std::endl;
    }
    for(int i=0; i<50; i++){
        std::cout << "Got: " << myqueue.get() << std::endl;
    }
}
