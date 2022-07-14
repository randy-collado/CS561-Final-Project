CC = g++
CFLAGS = -Wall -O3 -march=native -std=c++17 -fopenmp -D_GNU_SOURCE
OBJS = Tree.o TreeAlgorithm.o Serializer.o Graph.o GraphAlgorithm.o

build: $(OBJS) main.o TreeGenerator.o
	$(CC) $(CFLAGS) $(OBJS) main.o -o main
	$(CC) $(CFLAGS) $(OBJS) TreeGenerator.o -o generator

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Serializer.o: Serializer.cpp Serializer.hpp
	$(CC) $(CFLAGS) -c Serializer.cpp

Tree.o: Tree.cpp Tree.hpp
	$(CC) $(CFLAGS) -c Tree.cpp

TreeAlgorithm.o: TreeAlgorithm.cpp TreeAlgorithm.hpp
	$(CC) $(CFLAGS) -c TreeAlgorithm.cpp

TreeGenerator.o: TreeGenerator.cpp
	$(CC) $(CFLAGS) -c TreeGenerator.cpp

Graph.o: Graph.cpp Graph.hpp
	$(CC) $(CFLAGS) -c Graph.cpp

GraphAlgorithm.o: GraphAlgorithm.cpp GraphAlgorithm.hpp
	$(CC) $(CFLAGS) -c GraphAlgorithm.cpp

diotest:
	$(CC) -g $(CFLAGS) -pthread diotest.cpp -o diotest

clean:
	del *.o
	del *.exe
