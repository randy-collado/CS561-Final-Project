CC = clang++
CFLAGS = -g -msse2 -msse3 -std=c++17

treefile: TreeSerializer.cpp main.cpp
	$(CC) $(CFLAGS) TreeSerializer.cpp main.cpp -o treefile

build: main.cpp
	$(CC) $(CFLAGS) -std=c++17 main.cpp -o main

del:
	rm treefile

clear:
	rm main
	rm -rf *.dSYM
