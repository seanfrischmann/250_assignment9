# Makefile for the AVL tree assignment

OBJS = term_control.o error_handling.o printtree.o main.o
CC = g++
DEBUG = -g
CFLAGS = -Wall $(DEBUG)
LFLAGS = -Wall $(DEBUG)

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o avltest

main.o: main.cpp error_handling.h term_control.h AVLTree.h AVLTree.cpp AVLremove.cpp
	$(CC) -c $(CFLAGS) main.cpp

printtree.o: term_control.o error_handling.o printtree.cpp
	$(CC) -c $(CFLAGS) printtree.cpp

error_handling.o : term_control.h error_handling.h error_handling.cpp
	$(CC) -c $(CFLAGS) error_handling.cpp

term_control.o : term_control.h term_control.cpp
	$(CC) -c $(CFLAGS) term_control.cpp

clean:
	rm -f *.o a.out main avltest
