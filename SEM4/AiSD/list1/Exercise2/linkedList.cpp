#include <iostream>
#include "../GeneralTester.h"

struct Node {
    public:
    Node *next;
    int value;
    Node(int value){
        this->value=value;
        this->next=nullptr;
    }
};

class linkedList {
    public:
    Node *first;
    Node *last;
    int size;
    linkedList(): first(nullptr), last(nullptr), size(0){}
    int getSize(){return this->size;}
    Node* getByIndex(int index){
        if(index>size){
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
            first->next=first;
            last=first;
        }else{
            last->next=new Node(x);
            last=last->next;
            last->next=first;
        }
        size++;
    }
    void insertAt(int value, int index){
        if(index>size){
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
        previous->next=addedNode;
        addedNode->next=current;
        size++;
    }
    void deleteAt(int index){
        if(index>=size){
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
        previous->next=current->next;
        if(first==current){
            first=previous->next;
        }
        if(last==current){
            last=previous;
        }
        delete current;
        size--;
    }
    ~linkedList(){
        int i=0;
        int n = size;
        while(i<n){
            deleteAt(0);
            i++;
        }
    }
    int find(int value){
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
};

/*firsst element is taken from list 1*/
class Merger {
    public:
    linkedList* merge(linkedList *l1, linkedList *l2){
        linkedList *created = new linkedList();

        Node *temp = l1->first;
        Node *node = new Node(l1->first->value);
        Node *previous;
        created->first=node;
        for(int i=0; i<l1->size-1; i++){
            temp=temp->next;
            previous=node;
            node=new Node(temp->value);
            previous->next=node;
        }
        temp=l2->last;
        for(int i=0; i<l2->size; i++){
            temp=temp->next;
            previous=node;
            node=new Node(temp->value);
            previous->next=node;
        }
        node->next=created->first;
        
        created->size=l1->size+l2->size;
        return created;
    }
};

class Tester{
    public:
    void testMerger(){
        linkedList l1;
        linkedList l2;
        std:: cout << "list 1: ";
        for(int i=10; i<=20;i++){
            l1.insert(i);
            std:: cout <<i <<" ";
        }
        std:: cout << std::endl << "list 2: ";
        for(int i=20; i<=30;i++){
            l2.insert(i);
            std:: cout <<i <<" ";
        }
        std::cout << std::endl;
        Merger merger;
        linkedList *created = merger.merge(&l1,&l2);
        Node *temp=created->first;
        for(int i=0; i<created->size; i++){
            std::cout << "value: " << temp->value << "next value:" << temp->next->value << std::endl;
            temp = temp->next;
        }
    }
    void testFind(){
        GeneralTester tester;
        int *randomArray = tester.randomArrayGenerator(10000);
        linkedList list;
        for(int i=0; i<10000; i++){
            list.insert(randomArray[i]);
        }
        int average = 0;
        for(int i=0; i<1000; i++){
            average+=list.find(randomArray[tester.randomNumber(10000)]);
        }
        double result = average/1000;
        std::cout<<result<<std::endl;
    }
    void testFindExtended(){
        GeneralTester tester;
        int *randomArray = tester.randomArrayGenerator(10000);
        linkedList list;
        for(int i=0; i<10000; i++){
            list.insert(randomArray[i]);
        }
        int average = 0;
        for(int i=0; i<1000; i++){
            average+=list.find(tester.randomNumber(10001));
        }
        double result = average/1000;
        std::cout<<result<<std::endl;
    }
};

int main(){
    Tester tester;
    tester.testMerger();
    tester.testFind();
    tester.testFindExtended();
}
