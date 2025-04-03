#include <iostream>
#include "../GeneralTester.h"

struct Node {
    public:
    Node *next;
    Node *previous;
    int value;
    Node(int value){
        this->value=value;
        this->next=nullptr;
        this->previous=nullptr;
    }
};

class Connecter{
    public:
    void connect(Node *node1, Node *node2){
        node1->next=node2;
        node2->previous=node1;
    }
};

class doubleLinkedList {
    private:
    int findNext(int value){
        if(first==nullptr){
            throw std::runtime_error("empty list");
        }
        int cmps =0;
        Node *current = first;
        do {
            cmps++;
            if(current->value==value){
                return cmps;
            }
            current=current->next;
        } while(current!=first);
        return cmps;
    }
    int findPrevious(int value){
        if(first==nullptr){
            throw std::runtime_error("empty list");
        }
        int cmps =0;
        Node *current = first;
        do {
            cmps++;
            if(current->value==value){
                return cmps;
            }
            current=current->previous;
        } while(current!=first);
        return cmps;
    }
    public:
    Node *first;
    Node *last;
    int numberOfElements;
    int find(int value, int type){
        if(type==0){
            return findNext(value);
        }else if(type==1){
            return findPrevious(value);
        }
        return 0;
    }
    doubleLinkedList(): first(nullptr), last(nullptr), numberOfElements(0){}
    int getSize(){return this->numberOfElements;}
    Node* getByIndex(int index){
        if(index>numberOfElements){
            throw std::runtime_error("index greater than size of the list");
        }
        int i=0;
        Node *temp = first;
        while(i<index){
            temp=temp->next;
            i++;
        }
        return temp;
    }
    void insert(int x){
        if(first==nullptr){
            first=new Node(x);
            Connecter connecter;
            connecter.connect(first, first);
            last=first;
        }else{
            Node *addedNode = new Node(x);
            last->next=addedNode;
            Connecter connecter;
            connecter.connect(last, addedNode);
            connecter.connect(addedNode, first);
            last=last->next;
        }
        numberOfElements++;
    }
    void insertAt(int value, int index){
        if(index>numberOfElements){
            throw std::runtime_error("index greater than size of the list");
        }
        int i=0;
        Node *previous = last;
        Node *current = first;
        while(i<index){
            previous=current;
            current=current->next;
            i++;
        }
        Node* addedNode = new Node(value);
        Connecter connector;
        connector.connect(previous, addedNode);
        connector.connect(addedNode, current);
        numberOfElements++;
    }
    void deleteAt(int index){
        if(index>=numberOfElements){
            throw std::runtime_error("index greater than size of the list");
        }
        int i=0;
        Node *previous = last;
        Node *current = first;
        while(i<index){
            previous=current;
            current=current->next;
            i++;
        }
        Connecter connecter;
        connecter.connect(previous, current->next);
        previous->next=current->next;
        current->next->previous=previous;
        if(first==current){
            first=previous->next;
        }
        if(last==current){
            last=previous;
        }
        delete current;
        numberOfElements--;
    }
    ~doubleLinkedList(){
        int i=0;
        int n = numberOfElements;
        while(i<n){
            deleteAt(0);
            i++;
        }
    }
};

/*firsst element is taken from list 1*/
class Merger {
    public:
    doubleLinkedList* merge(doubleLinkedList *l1, doubleLinkedList *l2){
    doubleLinkedList *created = new doubleLinkedList();

        Connecter connecter;
        Node *temp = l1->first;
        Node *node = new Node(l1->first->value);
        Node *previous;
        created->first=node;
        for(int i=0; i<l1->numberOfElements-1; i++){
            temp=temp->next;
            previous=node;
            node=new Node(temp->value);
            connecter.connect(previous, node);
        }
        temp=l2->last;
        for(int i=0; i<l2->numberOfElements; i++){
            temp=temp->next;
            previous=node;
            node=new Node(temp->value);
            connecter.connect(previous, node);
        }
        connecter.connect(node, created->first);
        
        created->numberOfElements=l1->numberOfElements+l2->numberOfElements;
        return created;
    }
};

class Tester{
    public:
    void basicTest(){
        doubleLinkedList l1;
        doubleLinkedList l2;
        for(int i=0; i<10;i++){
            l1.insert(i);
            l2.insert(i);
        }
        Merger merger;
        doubleLinkedList *created = merger.merge(&l1,&l2);
        Node *temp=created->first;
        for(int i=0; i<created->numberOfElements; i++){
            std::cout << "value: " << temp->value << "next value:" << temp->next->value << "previous value: " <<temp->previous->value << std::endl;
            temp = temp->next;
        }
    }
    void testFind(){
        GeneralTester tester;
        int *randomArray = tester.randomArrayGenerator(10000);
        doubleLinkedList list;
        for(int i=0; i<10000; i++){
            list.insert(randomArray[i]);
        }
        int average = 0;
        for(int i=0; i<1000; i++){
            average+=list.find(randomArray[tester.randomNumber(1000)],tester.randomBin());
        }
        double result = average/1000;
        std::cout<<result<<std::endl;
    }
    void testFindExtended(){

        GeneralTester tester;
        int *randomArray = tester.randomArrayGenerator(10000);
        doubleLinkedList list;
        for(int i=0; i<10000; i++){
            list.insert(randomArray[i]);
        }
        int average = 0;
        for(int i=0; i<1000; i++){
            average+=list.find(tester.randomNumber(10001),tester.randomBin());
        }
        double result = average/1000;
        std::cout<<result<<std::endl;
    }
};

int main(){
    Tester tester;
    tester.basicTest();
    tester.testFind();
    tester.testFindExtended();
}
