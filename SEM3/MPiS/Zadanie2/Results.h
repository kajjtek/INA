#ifndef RESULTS_H
#define RESULTS_H

class Results {
    private:
        int firstCollision=0;
        int emptyBoxes=0;
        int atLeastOne=0;
        int atLeastTwo=0;
        int difference=0;

    public:
        void setfirstCollision(int m);
        
        void setemptyBoxes(int m);

        void setatLeastOne(int m);

        void setatLeastTwo(int m);

        void setDifference();

        void printResults();
    
};


#endif