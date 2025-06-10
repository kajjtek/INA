#ifndef SOLUTIONFINDER_H
#define SOLUTIONFINDER_H

#include <vector>
#include "GameBoard.h"

class SolutionFinder {
    public:
    int findBestMove(int depth, int player, GameBoard &gameBoard);
    private:
    int calculateHeurestic(const GameBoard &gameBoard);
    int findMax(int depth, GameBoard gameBoard, int alfa, int beta);
    int findMin(int depth, GameBoard gameBoard, int alfa, int beta);
    std::vector<int> getValidMoves(const GameBoard &gameBoard);
    int calculateHorizontalPatterns(const GameBoard &gameBoard);
    int calculateVerticalPatterns(const GameBoard &gameBoard);
    int calculateDiagonalPatterns(const GameBoard &gameBoard);
    int doCalculate(int x, int y, int z, int d, int shift);
    int case00(std::vector<int> seq);
    int casex0(std::vector<int> seq, int player, int shift);
    int case0x(std::vector<int> seq, int player, int shift);
    int casexx(std::vector<int> seq, int player, int shift);
    int casexy(std::vector<int> seq);
    int winningPossibilitiesHeurestic(const GameBoard &gameBoard);
};

#endif