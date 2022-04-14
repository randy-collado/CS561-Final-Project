#include <iostream>
#include <vector>
#include "omp.h"

#pragma once

enum MODE {READ, WRITE, INVALID};


struct dummy_node { int x; int y; int z; };
struct S_Node {
	int id;
	int payloads[8];
	int children[8]; //id or keys of the nodes

} __attribute__ ((aligned(512))); //GCC extension to align a struct
class TreeSerializer{
	public:
		TreeSerializer();
		~TreeSerializer();
		void writeNode(S_Node* node);
		void openFile(std::string filepath, MODE mode);
		S_Node* readNode();
	private:
		int fd;
		MODE mode_internal;
		size_t offset;
		bool at_eof;
		size_t read_offset;
};



