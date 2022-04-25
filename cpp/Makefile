CC = g++
CFLAGS = -Wall -O3 -std=c++17 -fopenmp
OBJS = main.o algorithm.o tree.o TreeSerializer.o

build: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main.exe

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

TreeSerializer.o: TreeSerializer.cpp TreeSerializer.hpp
	$(CC) $(CFLAGS) -c TreeSerializer.cpp

algorithm.o: algorithm.cpp algorithm.hpp
	$(CC) $(CFLAGS) -c algorithm.cpp

tree.o: tree.cpp tree.hpp
	$(CC) $(CFLAGS) -c tree.cpp

clean:
	del *.o
	del *.exe
