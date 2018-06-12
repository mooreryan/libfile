CC = gcc

ifeq ($(OPTIMIZE),)
OPTIMIZE = 3
endif

CFLAGS = -Wall -g -O$(OPTIMIZE)

BIN = bin
SRC = src
OBJ = $(SRC)/obj
OBJS := $(OBJ)/bstrlib.o $(OBJ)/rstring.o $(OBJ)/rfile.o

BSTRING_SRC = vendor/bstrlib

DEPENDS := bstrlib rstring rfile

.PHONY: clean

clean:
	rm $(OBJ)/*.o

main: $(OBJS)
	$(CC) $(CFLAGS) -I$(OBJ) -I$(BSTRING_SRC) -o $(BIN)/$@ $(SRC)/$@.c $^

test_main: main
	valgrind --leak-check=full $(BIN)/main

$(OBJ)/bstrlib.o:
	$(CC) $(CFLAGS) -c -o $@ $(BSTRING_SRC)/bstrlib.c

$(OBJ)/rstring.o:
	$(CC) $(CFLAGS) -I$(OBJ) -I$(BSTRING_SRC) -c -o $@ $(SRC)/rstring.c

$(OBJ)/rfile.o:
	$(CC) $(CFLAGS) -I$(OBJ) -I$(BSTRING_SRC) -c -o $@ $(SRC)/rfile.c

ceedling:
	rm build/test/out/*; ceedling && parallel "echo;echo;echo;echo;echo;echo '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'; valgrind --leak-check=full {}" ::: build/test/out/*.out
