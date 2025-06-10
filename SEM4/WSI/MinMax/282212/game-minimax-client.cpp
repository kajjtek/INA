#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "GameBoard.h"
#include "SolutionFinder.h"

int main(int argc, char *argv[]) {
  int server_socket;
  struct sockaddr_in server_addr;
  char server_message[16], player_message[16];

  bool end_game;
  int player;
  int msg;
  int move;

  GameBoard gameBoard_instance;
  SolutionFinder ai_solver;

  if (argc != 6) {
    std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port> <player_id> <player_name> <depth> \n";
    return -1;
  }

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    std::cerr << "Error: Unable to create socket\n";
    return -1;
  }
  std::cout << "Socket created successfully\n";

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(std::stoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  int ai_search_depth = std::stoi(argv[5]);

  if (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Error: Unable to connect to server\n";
    close(server_socket);
    return -1;
  }
  std::cout << "Connected with server successfully\n";

  memset(server_message, '\0', sizeof(server_message));
  if (recv(server_socket, server_message, sizeof(server_message), 0) < 0) {
    std::cerr << "Error while receiving server's initial message\n";
    close(server_socket);
    return -1;
  }

  memset(player_message, '\0', sizeof(player_message));
  snprintf(player_message, sizeof(player_message), "%s %s", argv[3], argv[4]);
  if (send(server_socket, player_message, strlen(player_message), 0) < 0) {
    std::cerr << "Error: Unable to send player info to server\n";
    close(server_socket);
    return -1;
  }
//i used it in constructor
//   gameBoard_instance.setBoard();
  end_game = false;
  player = std::stoi(argv[3]);

  std::cout << "Minimax AI Client initialized as Player " << player << ".\n";

  while (!end_game) {
    memset(server_message, '\0', sizeof(server_message));
    if (recv(server_socket, server_message, sizeof(server_message), 0) < 0) {
      std::cerr << "Error while receiving server's message during game loop\n";
      end_game = true;
      continue;
    }

    sscanf(server_message, "%d", &msg);
    move = msg % 100;
    msg = msg / 100;

    if (move != 0) {
      if (!gameBoard_instance.setMove(move, 3 - player)) {
        std::cerr << "Error: Server sent an invalid opponent move (" << move << ").\n";
        end_game = true;
        continue;
      }
    }

    if ((msg == 0) || (msg == 6)) {
      int bestMove = 0;
      try {
        bestMove = ai_solver.findBestMove(ai_search_depth, player, gameBoard_instance);
      } catch (const std::runtime_error& e) {
        std::cerr << "AI Error: " << e.what() << std::endl;
        end_game = true;
        continue;
      }

      if (!gameBoard_instance.setMove(bestMove, player)) {
        std::cerr << "Error: AI generated an invalid move (" << bestMove << ").\n";
        end_game = true;
        continue;
      }

      memset(player_message, '\0', sizeof(player_message));
      snprintf(player_message, sizeof(player_message), "%d", bestMove);
      if (send(server_socket, player_message, strlen(player_message), 0) < 0) {
        std::cerr << "Error: Unable to send AI move to server\n";
        end_game = true;
        continue;
      }

    } else {
      end_game = true;
      switch (msg) {
        case 1: std::cout << "Game Over: You won.\n"; break;
        case 2: std::cout << "Game Over: You lost.\n"; break;
        case 3: std::cout << "Game Over: Draw.\n"; break;
        case 4: std::cout << "Game Over: You won. Opponent error.\n"; break;
        case 5: std::cout << "Game Over: You lost. Your error.\n"; break;
        default: std::cout << "Game Over: Unknown message type (" << msg << ").\n"; break;
      }
    }
  }

  close(server_socket);
  std::cout << "Socket closed. Game process terminated.\n";

  return 0;
}
