#include "Tree.cpp"
#include <iostream>
#include <vector>
#include "omp.h"

enum MODE {READ, WRITE};

class TreeSerializer{
	public:
		TreeSerializer(std::string filepath, MODE mode);
		~TreeSerializer();
		void writeNode(Node* node);
		Node* readNode();
	private:
		int fd;
		MODE mode_internal;
};

