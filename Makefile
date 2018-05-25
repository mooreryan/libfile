CC = gcc

CFLAGS = -Wall -g

BIN = bin
SRC = src

OBJS := src/file.o

main: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $(SRC)/$@.c $^

test_main: main
	valgrind --leak-check=full $(BIN)/main

ceedling:
	ceedling && parallel "echo;echo;echo;echo;echo;echo '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'; valgrind --leak-check=full {}" ::: build/test/out/*.out
