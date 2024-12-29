#include <stdio.h>
#include <iostream>
#include "Results.h"
#include "Simulation.h"
#include <random>


using namespace std;

            std::random_device rd;
            std::mt19937 generator(rd());

        void Simulation::reset(){
            populateBoxes();
            this->index_m = 1;
            this->oneball = 0;
            this->twoball = 0; 
            this->flagone = 0;
            this->flagtwo = 0;
            this->recorder = 0;
            this->maxIndex=0;
        }

        void Simulation::populateBoxes(){
            this->boxes.clear();
            this->boxes.resize(this->n,0);
        }

        void Simulation::incrementIndex_M(){
            this->index_m++;
        }

        void Simulation::setN(int x){
            this->n=x;
            std::uniform_int_distribution<int> distribution(0, (this->n)-1);
        }
        
        void Simulation::incrementOneBall(){
            this->oneball++;
        }

        void Simulation::incrementTwoball(){
            this->twoball++;
        }

        void Simulation::incrementFirstFlag(){
            this->flagone++;
        }

        void Simulation::incrementSecondFlag(){
            this->flagtwo++;
        }

        void Simulation::incrementRecorder(){
            this->recorder++;
        }

        void Simulation::verifyMax(int index){
            if(this->boxes[index]>this->boxes[this->maxIndex]){
                this->maxIndex=index;
            }
        }


        Results Simulation::executeSimulation(){
            Results results;
            std::uniform_int_distribution<int> distribution(0, (this->n)-1);
            while (this->twoball < this->n)
            {
                int temp = distribution(generator);
                this->boxes[temp]++;
                if (this->boxes[temp]==1)
                {
                    incrementOneBall();
                }
                if (this->boxes[temp]==2)
                {
                    if(this->recorder==0){
                        incrementRecorder();
                        results.setfirstCollision(this->index_m);
                    }
                    incrementTwoball();
                }
                if(this->oneball==n && this->flagone==0){
                    incrementFirstFlag();
                    results.setatLeastOne(this->index_m);
                }
                if(this->twoball==n && this->flagtwo==0){
                    incrementSecondFlag();
                    results.setatLeastTwo(this->index_m);
                }
                if(this->index_m==this->n){
                    results.setemptyBoxes(this->n-this->oneball);
                }
                incrementIndex_M();
            }
            results.setDifference();
            return results;
        }
        
int Simulation::MinimumInsertion(int d, std::uniform_int_distribution<int> dis){
            int minimumValue=INT_MAX;
            int minIndex;
            for (int i = 0; i < d; i++)
            {
                int temp = dis(generator);
                if(this->boxes[temp]<minimumValue){
                    minimumValue=this->boxes[temp];
                    minIndex=temp;
                }
            }
            return minIndex;
}       

Results Simulation::executeSimulationEX3(){
            Results results;
            std::uniform_int_distribution<int> distribution(0, (this->n)-1);
            while (this->index_m <= this->n)
            {
                int temp = MinimumInsertion(2, distribution);
                this->boxes[temp]++;
                verifyMax(temp);
                if(this->index_m==this->n){
                    results.setMaxValue(this->boxes[this->maxIndex]);
                }
                incrementIndex_M();
            }
            return results;
        }
