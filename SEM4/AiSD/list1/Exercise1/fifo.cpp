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
class fifo {
    private:
    Node<T> *front;
    Node<T> *rear;
    public:
    fifo():front(nullptr), rear(nullptr){}
    void add(T value){
        if(front==nullptr){
            Node<T>* addedNode = new Node<T>(value);
            front=addedNode;
            rear=front;
        }else{
            Node<T>* addedNode = new Node<T>(value);
            rear->setNext(addedNode);
            rear=addedNode;
        }
    }
    T get(){
        if(front==nullptr){
            throw std::runtime_error("queue is empty");
        }
        else{
            Node<T> *temp = front;
            front = front->getNext();
            return temp->getValue();
        }
    }
    ~fifo(){
        while(!(front==nullptr)){
            get();
        }
    }
};

int main(){
    fifo<int> myqueue;
    for(int i=0; i<50;i++){
        myqueue.add(i);
        std::cout << "Input: " << i <<std::endl;
    }
    for(int i=0; i<50; i++){
        std::cout << "Got: " << myqueue.get() << std::endl;
    }
}
