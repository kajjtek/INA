#ifndef STRINGMATCHING_H
#define STRINGMATCHING_H
#include <string>

class StringMatching {
    public:
    void FA(std::string line, std::string pattern, int row);
    void KMP(std::string line, std::string pattern, int row);
    void betterFA(std::string line, std::string pattern, int row);
    private:
    int** preprocessingFA(int** transition_table, std::string pattern);
    int** preprocessingBetterFA(int** transition_table, int* pi, std::string pattern);
    int* preprocessingKMP(int* transition_table, std::string pattern);
};

#endif