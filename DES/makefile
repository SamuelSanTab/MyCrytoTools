CC := gcc
CFLAGS := -std=c2x -W -Wall -g
TEST := des_test

all: $(TEST) clean

des_test: des.o
	@echo "----------------------------"
	@echo "Test de des.c"
	@echo "----------------------------"
	$(CC) $(CFLAGS) test/des_test.c $^ -o $@
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$@ || true
	rm -f $@
	@echo "----------------------------"

des.o:
	$(CC) $(CFLAGS) des.c -c

clean:
	rm -f *.o $(TEST)