#include <stdio.h>
#include <iostream>
#include "Results.h"
#include "Simulation.h"
#include <random>


using namespace std;

        void Simulation::populateBoxes(){
            this->boxes.resize(this->n,0);
        }

        void Simulation::incrementIndex_M(){
            this->index_m++;
        }

        void Simulation::setN(int x){
            this->n=x;
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

        void Simulation::setM(int x){
            this->m=x;
        }

        void Simulation::executeSimulation(){
            Results results;
            std::random_device rd;
            std::mt19937 generator(rd());
            std::uniform_int_distribution<int> distribution(0, (this->n)-1);
            while (this->index_m <= this->m)
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
                incrementIndex_M();
            }
            results.setDifference();
            results.printResults();
        }
        