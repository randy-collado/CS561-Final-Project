CC = g++
CFLAGS = -Wall -O3 -std=c++17
OBJS = main.o algorithm.o tree.o # treefile.o

build: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -fopenmp -o main.exe

main.o: main.cpp
	$(CC) $(CFLAGS) -c -fopenmp main.cpp

# treefile.o: TreeSerializer.cpp
# 	$(CC) $(CFLAGS) -c TreeSerializer.cpp

algorithm.o: algorithm.cpp
	$(CC) $(CFLAGS) -c -fopenmp algorithm.cpp

tree.o: tree.cpp
	$(CC) $(CFLAGS) -c tree.cpp

clean:
	del *.o
	del *.exe
