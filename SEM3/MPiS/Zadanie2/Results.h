#ifndef RESULTS_H
#define RESULTS_H

class Results {
    private:
        int firstCollision=0;
        int emptyBoxes=0;
        int atLeastOne=0;
        int atLeastTwo=0;
        int difference=0;
        int maxValue=0;

    public:

        void printMaxValue();

        int returnfirstCollision();

        int returnemptyBoxes();

        int returnatLeastOne();

        int returnatLeastTwo();

        int returnDifference();

        int returnMaxValue();

        void setfirstCollision(int m);
        
        void setemptyBoxes(int m);

        void setatLeastOne(int m);

        void setatLeastTwo(int m);

        void setDifference();

        void setMaxValue(int x);

        void printResults();
    
};


#endif