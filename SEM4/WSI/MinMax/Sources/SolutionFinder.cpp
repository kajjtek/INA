#include "../SolutionFinder.h"
#include "../GameBoard.h"
#include <limits>
#include <iostream>

int SolutionFinder::findBestMove(int depth, int player, GameBoard &gameBoard){
    int alfa = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();
    std::vector<int> possibleMoves = getValidMoves(gameBoard);

    if(possibleMoves.empty()) return -1; 
    int bestmove;
    if(player==1){
        int max = std::numeric_limits<int>::min();

        for(int move: possibleMoves) {
            GameBoard temp = gameBoard;
            if(!temp.setMove(move, 1)) throw std::runtime_error("Internal error - possibleMoves");
            int h = findMin(depth-1, temp, alfa, beta);
            if(h>=max){
                max = h;
                bestmove=move;
            } 
            alfa = std::max(alfa, h);
            if(alfa>=beta) return bestmove;
        }
    }
   else if(player==2){
        int min = std::numeric_limits<int>::max();

        for(int move: possibleMoves) {
            GameBoard temp = gameBoard;
            if(!temp.setMove(move, 2)) throw std::runtime_error("Internal error - possibleMoves");
            int h = findMax(depth-1, temp, alfa, beta); 
            if(h<=min){
                min = h;
                bestmove=move;
            }
            beta = std::min(beta,h);
            if(alfa>=beta) return bestmove;
        }
    }
    return bestmove;
}

int SolutionFinder::findMax(int depth, GameBoard gameBoard, int alfa, int beta){
    if(gameBoard.winCheck(1)) return std::numeric_limits<int>::max();
    if(gameBoard.loseCheck(1)) return std::numeric_limits<int>::min();

    if(depth==0) return calculateHeurestic(gameBoard);

    std::vector<int> possibleMoves = getValidMoves(gameBoard);
    

    if(possibleMoves.empty()) return calculateHeurestic(gameBoard); //this means it is a draw as noone has won and there are no more moves to be done
    int max = std::numeric_limits<int>::min();

    for(int move: possibleMoves) {
        GameBoard temp = gameBoard;
        if(!temp.setMove(move, 1)) throw std::runtime_error("Internal error - possibleMoves");
        int h = findMin(depth-1, temp, alfa, beta); 
        max = std::max(max, h);
        alfa = std::max(alfa, h);
        if(alfa>=beta) return max;
    }
    return max;
}

int SolutionFinder::findMin(int depth, GameBoard gameBoard, int alfa, int beta){
    if(gameBoard.winCheck(2)) return std::numeric_limits<int>::min();
    if(gameBoard.loseCheck(2)) return std::numeric_limits<int>::max();

    if(depth==0) return calculateHeurestic(gameBoard);

    std::vector<int> possibleMoves = getValidMoves(gameBoard);

    if(possibleMoves.empty()) return calculateHeurestic(gameBoard); //this means it is a draw as noone has won and there are no more moves to be done
    int min = std::numeric_limits<int>::max();

    for(int move: possibleMoves) {
        GameBoard temp = gameBoard;
        if(!temp.setMove(move, 2)) throw std::runtime_error("Internal error - possibleMoves");
        int h = findMax(depth-1, temp, alfa, beta); 
        min = std::min(min,h);
        beta = std::min(beta,h);
        if(alfa>=beta) return min;
    }
    return min;
}

std::vector<int> SolutionFinder::getValidMoves(const GameBoard &gameBoard){
    std::vector<int> temp;
   for(int i=0;i<5; i++){
    for(int j=0; j<5; j++){
        if(gameBoard.board[i][j]==0){
            temp.push_back((i+1)*10+(j+1));
        }
    }
   }
    return temp;
}

/*
Heuristic:
- existence of two 1s next to each other +100
- existence of two 2s next to each other -100
- existence of one 1/2 next to each other +1/-1
- existence of three 1s in line MIN_INT
- existence of three 2s in line MAX_INT
- existence of 1_11 or 11_1 +10000
- existence of 2_22 or 22_2 -10000
- existence of 1__1 +1000
- existence of 2__2 -1000
- existence of 1111 MAX_INT
- existence of 2222 MIN_INT
This will have one coefficient
We will also add second heurisitc that checks how many possible win variants are still possible - later
*/

int SolutionFinder::calculateHeurestic(const GameBoard &gameBoard){
    int score = 0;
    score +=calculateHorizontalPatterns(gameBoard);
    score +=calculateVerticalPatterns(gameBoard);
    score +=calculateDiagonalPatterns(gameBoard);
    return score;
}

int SolutionFinder::calculateHorizontalPatterns(const GameBoard &gameBoard){
    int score = 0;
    for(int row=0; row<5; row++){
        for(int col=0; col<2; col++){
            int tile1 = gameBoard.board[row][col];
            int tile2 = gameBoard.board[row][col+1];
            int tile3 = gameBoard.board[row][col+2];
            int tile4 = gameBoard.board[row][col+3];
            score+=doCalculate(tile1,tile2,tile3,tile4, col);
        }
    }
    return score;
}

int SolutionFinder::calculateVerticalPatterns(const GameBoard &gameBoard){
    int score = 0;
    for(int col=0; col<5; col++){
        for(int row=0; row<2; row++){
            int tile1 = gameBoard.board[row][col];
            int tile2 = gameBoard.board[row+1][col];
            int tile3 = gameBoard.board[row+2][col];
            int tile4 = gameBoard.board[row+3][col];
            score+=doCalculate(tile1,tile2,tile3,tile4, row);
        }
    }
    return score;
}

int SolutionFinder::calculateDiagonalPatterns(const GameBoard &gameBoard){
    int score = 0;
    score+=doCalculate(gameBoard.board[0][0],gameBoard.board[1][1],gameBoard.board[2][2],gameBoard.board[3][3],0);
    score+=doCalculate(gameBoard.board[1][1],gameBoard.board[2][2],gameBoard.board[3][3],gameBoard.board[4][4],1);
    score+=doCalculate(gameBoard.board[1][0],gameBoard.board[2][1],gameBoard.board[3][2],gameBoard.board[4][3],0);
    score+=doCalculate(gameBoard.board[0][1],gameBoard.board[1][2],gameBoard.board[2][3],gameBoard.board[3][4],0);
    score+=doCalculate(gameBoard.board[0][4],gameBoard.board[1][3],gameBoard.board[2][2],gameBoard.board[3][1],0);
    score+=doCalculate(gameBoard.board[3][1],gameBoard.board[2][2],gameBoard.board[1][3],gameBoard.board[0][4],1);
    score+=doCalculate(gameBoard.board[3][0],gameBoard.board[2][1],gameBoard.board[1][2],gameBoard.board[0][3],0);
    score+=doCalculate(gameBoard.board[4][1],gameBoard.board[3][2],gameBoard.board[2][3],gameBoard.board[1][4],0);
    return score;
}

int SolutionFinder::doCalculate(int tile1, int tile2, int tile3, int tile4, int shift){
    std::vector<int> sequence = {tile1, tile2, tile3, tile4};
    int count1 = 0;
    int count2 = 0;
    for(int i=1; i<=2; i++){
        if(sequence[i]==1) count1++;
        else if(sequence[i]==2) count2++;
    }
    if(count1==1 && count2==1) return casexy(sequence);
    else if(count1==1 && count2==0) return casex0(sequence, 1, shift);
    else if(count1==0 && count2==1) return casex0(sequence, 2, shift);
    else if(count1==0 && count2==0) return case00(sequence);
    else if(count1==1 && count2==0 && tile2==0) return case0x(sequence,1,shift);
    else if(count1==0 && count2==1 && tile2==0) return case0x(sequence,2,shift);
    else if(count1==2) return casexx(sequence, 1, shift);
    else if(count2==2) return casexx(sequence, 2, shift);
    else throw std::runtime_error("Problem with doCalculate");
}

int SolutionFinder::case00(std::vector<int> sequence){
    int count1 = 0;
    int count2 = 0;
    for(int i=0; i<=3; i+=3){
        if(sequence[i]==1) count1++;
        else if(sequence[i]==2) count2++;
    }
    if(count1==2) return 1000;
    else if(count2==2) return -1000;
    else return count1-count2;
}

int SolutionFinder::casexy(std::vector<int> sequence){
    int count1 = 0;
    int count2 = 0;
    for(int i=0; i<=3; i+=3){
        if(sequence[i]==1) count1++;
        else if(sequence[i]==2) count2++;
    }
    if(count1==2) return -10;
    else if(count2==2) return 10;
    else if(count1==1 && count2==1) return 0;//tu mozna pobawic sie shiftem jeszcze
    //reszta nie ma znaczenia bo my tu sie zawsze blokujemy - ewentualnie zwrocimy przewage pomiedzy bo wiecej 1 badz 2 daje wieksze mozliwosci
    else return count1-count2;
}

int SolutionFinder::casexx(std::vector<int> sequence, int player, int shift){
    int count1 = 0;
    int count2 = 0;
    player==1?count1+=2 : count2+=2;
    for(int i=0; i<=3; i+=3){
        if(sequence[i]==1) count1++;
        else if(sequence[i]==2) count2++;
    }
    if(count1==4) return std::numeric_limits<int>::max();
    else if(count2==4) return std::numeric_limits<int>::min();
    else if(count1==2 && count2==2) return 0;
    else if(count1==2 && count2==1 && sequence[0]==2 && shift==0) return 100;
    else if(count1==2 && count2==1 && sequence[0]==2) return -10;
    else if(count1==2 && count2==1 && sequence[4]==2 && shift==0) return -10;
    else if(count1==2 && count2==1 && sequence[4]==2) return 10;
    else if(count1==2 && count2==0) return 100;
    else if(count1==0 && count2==2) return -100;
    else return count1-count2;
}

int SolutionFinder::case0x(std::vector<int> sequence, int player, int shift){
    int count1 = 0;
    int count2 = 0;
    player==1?count1+=1 : count2+=1;
    for(int i=0; i<=3; i+=3){
        if(sequence[i]==1) count1++;
        else if(sequence[i]==2) count2++;
    }
    if(count1==3) return 10000;
    else if(count2==3) return -10000;
    else if(count1==2 && count2==0 && sequence[0]==1) return 100;//1010
    else if(count1==2 && count2==0 && sequence[0]==1) return 100;//0011
    else if(count1==0 && count2==2 && sequence[0]==2) return -100;//2020
    else if(count1==0 && count2==2 && sequence[4]==2) return -100;//0022
    else if(count1==1 && count2==0) return 1;
    else if(count1==1 && count2==1 && sequence[4]==2) return 0;
    else if(count1==1 && count2==1 && sequence[0]==2 && shift==0) return 5;
    else if(count1==0 && count2==1) return -1;
    else if(count1==1 && count2==1 && sequence[4]==1) return 0;
    else if(count1==1 && count2==1 && sequence[0]==1 && shift==0) return -5;
    else return count1-count2;
}

int SolutionFinder::casex0(std::vector<int> sequence, int player, int shift){
    int count1 = 0;
    int count2 = 0;
    player==1?count1+=1 : count2+=1;
    for(int i=0; i<=3; i+=3){
        if(sequence[i]==1) count1++;
        else if(sequence[i]==2) count2++;
    }
    if(count1==3) return 10000;
    else if(count2==3) return -10000;
    else if(count1==2 && count2==0 && sequence[0]==1) return 100;//1100
    else if(count1==2 && count2==0 && sequence[0]==1) return 100;//0101
    else if(count1==0 && count2==2 && sequence[0]==2) return -100;//2200 mozna pododawac shifty
    else if(count1==0 && count2==2 && sequence[4]==2) return -100;//0202
    else if(count1==1 && count2==0) return 1;
    else if(count1==1 && count2==1 && sequence[4]==2) return 0;// 0102
    else if(count1==1 && count2==1 && sequence[0]==2 && shift==0) return 5; //2100
    else if(count1==0 && count2==1) return -1;
    else if(count1==1 && count2==1 && sequence[4]==1) return 0;
    else if(count1==1 && count2==1 && sequence[0]==1 && shift==0) return -5;
    else return count1-count2;
}