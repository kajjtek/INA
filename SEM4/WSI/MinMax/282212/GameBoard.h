#ifndef GAMEBOARD_H
#define GAMEBOARD_H

class GameBoard {
    public:
    int board[5][5];
    static const int win[28][4][2];
      static const int lose[48][3][2];
      GameBoard();
      void setBoard();
      void printBoard();
      bool setMove(int move, int player);
      bool winCheck(int player);
      bool loseCheck(int player);
      int winCounter(int player) const;
};

#endif