CC = g++
CFLAGS = -Wall -O3 -std=c++17 -fopenmp -D_GNU_SOURCE
OBJS = main.o algorithm.o Tree.o TreeSerializer.o

generator: algorithm.o Tree.o TreeSerializer.o TreeGenerator.o
	$(CC) $(CFLAGS) algorithm.o Tree.o TreeSerializer.o TreeGenerator.o -o generator

build: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

TreeSerializer.o: TreeSerializer.cpp TreeSerializer.hpp
	$(CC) $(CFLAGS) -c TreeSerializer.cpp

algorithm.o: algorithm.cpp algorithm.hpp
	$(CC) $(CFLAGS) -c algorithm.cpp

tree.o: Tree.cpp Tree.hpp
	$(CC) $(CFLAGS) -c Tree.cpp

TreeGenerator.o: TreeGenerator.cpp
	$(CC) $(CFLAGS) -c TreeGenerator.cpp

diotest:
	g++ -g -msse2 -msse3 -std=c++17  -fopenmp -pthread diotest.cpp -o diotest -D_GNU_SOURCE

clean:
	del *.o
	del *.exe
