CC = gcc
CFLAGS = -Wall -g -pedantic -I./lib

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRC_FILES = $(SRC_DIR)/lsh.c $(SRC_DIR)/reader.c $(SRC_DIR)/parser.c $(SRC_DIR)/builtin.c $(SRC_DIR)/pipe.c
OBJ_FILES = $(OBJ_DIR)/lsh.o $(OBJ_DIR)/reader.o $(OBJ_DIR)/parser.o $(OBJ_DIR)/builtin.o $(OBJ_DIR)/pipe.o
TARGET = $(BIN_DIR)/lsh

all:$(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET)

 $(OBJ_DIR)/lsh.o: $(SRC_DIR)/lsh.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/lsh.c -o $(OBJ_DIR)/lsh.o


 $(OBJ_DIR)/reader.o: $(SRC_DIR)/reader.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/reader.c -o $(OBJ_DIR)/reader.o

 $(OBJ_DIR)/parser.o: $(SRC_DIR)/parser.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/parser.c -o $(OBJ_DIR)/parser.o

 $(OBJ_DIR)/builtin.o: $(SRC_DIR)/builtin.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/builtin.c -o $(OBJ_DIR)/builtin.o

 $(OBJ_DIR)/pipe.o: $(SRC_DIR)/pipe.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/pipe.c -o $(OBJ_DIR)/pipe.o

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/lsh
