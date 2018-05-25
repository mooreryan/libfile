CC = gcc

CFLAGS = -Wall -g

BIN = bin
SRC = src

OBJS := src/file.o

main: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $(SRC)/$@.c $^

test_main: main
	$(BIN)/main
