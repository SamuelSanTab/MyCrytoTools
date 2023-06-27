CC = gcc
CFLAGS = -std=c2x #-ansi -pedantic
MAIN = main

all: des.o
	$(CC) $(CFLAGS) test.c $^ -o $(MAIN)

des.o:
	$(CC) -c $(CFLAGS) des.c

clean:
	rm *.o $(MAIN)

test: all
	valgrind --leak-check=full -s ./$(MAIN)
	rm *.o $(MAIN)