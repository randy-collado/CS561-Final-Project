treefile:
	g++ -g -msse2 -msse3 -std=c++17 TreeSerializer.cpp main.cpp -o treefile

build: 
	g++ -g -msse2 -msse3 -std=c++17 main.cpp -o main

del:
	rm treefile

clear:
	rm main
	rm -rf *.dSYM

test:
	g++ -std=c++11 test.cpp -o test
del_test:
	rm test
