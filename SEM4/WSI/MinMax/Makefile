# Makefile for the game client and server

# Compiler for C files (server)
CC = gcc
# Compiler for C++ files (client)
CXX = g++

# Compiler flags for debugging
CFLAGS = -g -Wall -Wextra
CXXFLAGS = -g -Wall -Wextra -std=c++11

SERVER_SRC = game_server.c board.h
SERVER_OBJ = game_server.o
SERVER_EXEC = game_server

CLIENT_SRC = game-minimax-client.cpp GameBoard.cpp SolutionFinder.cpp GameBoard.h SolutionFinder.h
CLIENT_OBJ = game-minimax-client.o GameBoard.o SolutionFinder.o
CLIENT_EXEC = game-minimax-client

all: $(SERVER_EXEC) $(CLIENT_EXEC)

$(SERVER_EXEC): $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(SERVER_OBJ) -o $@

$(SERVER_OBJ): game_server.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) $(CLIENT_OBJ) -o $@

game-minimax-client.o: game-minimax-client.cpp GameBoard.h SolutionFinder.h
	$(CXX) $(CXXFLAGS) -O3 -c $< -o $@

GameBoard.o: ./Sources/GameBoard.cpp GameBoard.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

SolutionFinder.o: ./Sources/SolutionFinder.cpp SolutionFinder.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ) $(SERVER_EXEC) $(CLIENT_EXEC)