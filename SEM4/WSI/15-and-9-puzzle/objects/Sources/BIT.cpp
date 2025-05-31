#include "../BIT.h"

int BIT::getDefaultInversionsCount(std::vector<u_int8_t> array){
    int count = 0;
    int max = 0;

    for(int i=0; i<array.size(); i++){
        if(array.at(i)>max) max = array.at(i);
    }

    int tree[max+1];

    for(int i=1; i<=max; i++){
        tree[i] = 0;
    }

    for(int n=array.size()-1; n>=0; n--){
        uint8_t current = array.at(n);
        if(current==0x0) continue;

        count+=getSum(tree,current-1);
        update(tree, max, current, 1);
    }

    return count;
}

int BIT::getInversionsCount(State& state){
    int count=0;
    // int tree[16] is acceptable as a fixed-size array
    int tree[16];
    int max=15;
    for(int i=1; i<=max; i++){
        tree[i]=0;
    }

    for(int n=15; n>=0; n--){
        uint8_t current = state.get(n);
        if(current==0x0) continue;

        count+=getSum(tree,current-1);
        update(tree, max, current, 1);
    }

    return count;
}

int BIT::getSum(int tree[], int current){
    int x = current;
    int sum = 0;
    while(x>0){
        sum+=tree[x];
        x -= x & (-x);
    }
    return sum;
}

void BIT::update(int tree[], int n, int index, int value){
    while(index<=n){
        tree[index]+=value;
        index += index & (-index);
    }
}