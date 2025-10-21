#include "../header/StringMatching.h"
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>

void StringMatching::FA(std::string line, std::string pattern, int row){
    int** transition_table = new int*[pattern.size()+1];

    for(int i=0;i<=pattern.size();i++){
        transition_table[i] = new int[256];
    }

    transition_table = preprocessingFA(transition_table, pattern);

    int q = 0;
    for(int i=0; i<line.size(); i++){
        q = transition_table[q][(unsigned char)line[i]];

        if(q==pattern.size()){
            std::cout<<"Pattern found in row: "<<row<<" with starting index: "<<i-pattern.size()+1<<" end index: "<<i<<std::endl;
        }
    }

    for(int i=0;i<=pattern.size();i++){
        delete[] transition_table[i];
    }
    delete[] transition_table;
}

void StringMatching::betterFA(std::string line, std::string pattern, int row){
    int** transition_table = new int*[pattern.size()+1];

    for(int i=0;i<=pattern.size();i++){
        transition_table[i] = new int[256];
    }

    int* pi = new int[pattern.size()];

    preprocessingKMP(pi, pattern);

    transition_table = preprocessingBetterFA(transition_table, pi, pattern);

    int q = 0;
    for(int i=0; i<line.size(); i++){
        q = transition_table[q][(unsigned char)line[i]];

        if(q==pattern.size()){
            std::cout<<"Pattern found in row: "<<row<<" with starting index: "<<i-pattern.size()+1<<" end index: "<<i<<std::endl;
        }
    }

    for(int i=0;i<=pattern.size();i++){
        delete[] transition_table[i];
    }
    delete[] transition_table;
}

void StringMatching::KMP(std::string line, std::string pattern, int row){
    int* pi = new int[pattern.size()];//tu rozkminka

    pi = preprocessingKMP(pi, pattern);
    int q = 0;
    for(int i=0; i<line.size(); i++){
        while(q>0 && pattern[q]!=line[i]){
            q = pi[q-1];
        }

        if(pattern[q]==line[i]){
            q++;
        }

        if(q==pattern.size()){
            std::cout<<"Pattern found starting at index: "<<i-pattern.size()+1<<"ending at: "<<i<<std::endl;
            q = pi[q-1];
        }
    }

    delete[] pi;
}

int** StringMatching::preprocessingFA(int** transition_table, std::string pattern){
    for(int q=0; q<=pattern.size(); q++){
        for(int character=0; character<256; character++){
            int k = std::min({(int) pattern.size(), q+1});
            std::string concatenated = pattern.substr(0,q);
            concatenated += (char) character;
            while(!concatenated.ends_with(pattern.substr(0,k))){
                k--;
            }
            transition_table[q][(unsigned char)character]=k;
        }
    }
    return transition_table;
}

int** StringMatching::preprocessingBetterFA(int** transition_table, int* pi ,std::string pattern){

    for(int i=0; i<256; i++){
        char character = (char) i;
        if(character==pattern[0]){
            transition_table[0][(unsigned char) character]=1;
        }
        else transition_table[0][(unsigned char) character]=0;
    }

    for(int q=1; q<=pattern.size(); q++){
        for(int character=0; character<256; character++){
            if(q==pattern.size() || pattern[q]!=character){
                transition_table[q][(unsigned char) character] = transition_table[pi[q-1]][(unsigned char) character];
            }
            else transition_table[q][(unsigned char) character] = q+1;
        }
    }
    return transition_table;
}

int* StringMatching::preprocessingKMP(int* transition_table, std::string pattern){
    transition_table[0]=0;
    int k=0;

    for(int q=1; q<pattern.size();q++){
        while(k>0 && pattern[k]!=pattern[q]){
            k=transition_table[k-1];
        }

        if(pattern[k]==pattern[q]){
            k++;
        }
        transition_table[q]=k;
    }
    return transition_table;
}