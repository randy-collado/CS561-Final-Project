CC = g++
CFLAGS = -Wall -O3 -std=c++17 -fopenmp -D_GNU_SOURCE
OBJS = algorithm.o Tree.o Serializer.o

build: $(OBJS) main.o TreeGenerator.o
	$(CC) $(CFLAGS) $(OBJS) main.o -o main
	$(CC) $(CFLAGS) $(OBJS) TreeGenerator.o -o generator

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Serializer.o: Serializer.cpp Serializer.hpp
	$(CC) $(CFLAGS) -c Serializer.cpp

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
