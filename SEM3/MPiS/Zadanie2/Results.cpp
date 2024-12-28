#include <iostream>
#include "Results.h"

        void Results::printMaxValue(){
            std::cout << "Max value at this point is: " << this->maxValue << std::endl;
        }

        void Results::setMaxValue(int x){
            this->maxValue=x;
        }

        int Results::returnMaxValue(){
            return this->maxValue;
        }

        int Results::returnatLeastOne(){
            return this->atLeastOne;
        }

        int Results::returnatLeastTwo(){
            return this->atLeastTwo;
        }
        
        int Results::returnfirstCollision(){
            return this->firstCollision;
        }
        
        int Results::returnemptyBoxes(){
            return this->emptyBoxes;
        }

        int Results::returnDifference(){
            return this->difference;
        }

        void Results::setfirstCollision(int m){
            this->firstCollision=m;
        }
        
        void Results::setemptyBoxes(int m){
            this->emptyBoxes=m;
        }

        void Results::setatLeastOne(int m){
            this->atLeastOne=m;
        }

        void Results::setatLeastTwo(int m){
            this->atLeastTwo=m;
        }

        void Results::setDifference(){
            if(this->atLeastTwo!=0){
            this->difference=atLeastTwo-atLeastOne;
            }else{
                std::cout << "atLeast two equals zero" << std::endl;
            }
        }

        void Results::printResults(){
            std::cout << "First collision after: " << firstCollision << std::endl;
            std::cout << "Empty boxes: " << emptyBoxes << std::endl;
            std::cout << "At least one ball in every box: " << atLeastOne << std::endl;
            std::cout << "At least two balls in every box: " << atLeastTwo << std::endl;
            std::cout << "Difference: " << difference << std::endl;
        }
