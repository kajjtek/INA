#include <random>
#include <stdio.h>
#include <iostream>

class First{
    private:
    double probability;

    public:
    void executeSimulation();
    double getProbability();
};

double First::getProbability(){
    return this->probability;
}

void First::executeSimulation(){
    this->probability=0;
    double counter = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    int n;
    double fract;
    std::cout << "Symulacja obliczy prawdopodobienstwo gdzie X-Bin(n,1/2) P(X>=k) dla k = fract * E(X)" <<std::endl;
    std::cout << "Podaj n - wartosc opisujaca ilosc prob" <<std::endl;
    std::cin >> n;
    std::cout << "Podaj fract" <<std::endl;
    std::cin >> fract;
    std::binomial_distribution<> distribution(n, 0.5);
    double k = fract * n * 0.5; 

    for(int i=0; i<1000000000;i++){
        int result = distribution(gen);
        if(result>=k){
            counter++;
        }
    }

    this->probability=counter/1000000000;

    std::cout << "prawdopodobienstwo: " << this->probability << std::endl;
}