#define _GNU_SOURCE

#include "Tree.hpp"
#include "TreeSerializer.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <vector>

	/*
	 * NOTE: The size of a page on this device is 4096 bytes
	 */	
	TreeSerializer::TreeSerializer(std::string filename, MODE mode){
			int flags = O_DIRECT | O_SYNC;
			switch(mode){
				case MODE::READ: //READ
					flags |= O_RDONLY;
					mode_internal = mode;
					break;
				case MODE::WRITE: //WRITE
					flags |= O_RDWR | O_CREAT;
					mode_internal = mode;
					break;
				default: break;
			fd = open((filename + ".treefile").c_str(), flags);
			if (fd < 0) {
				std::cerr << "[ERROR]: Serializer failed\n[MESSAGE]: file unable to be opened or created";
				exit(1);
			}
		}
	}

	TreeSerializer::~TreeSerializer(){
		close(fd);
	}

	void TreeSerializer::writeNode(Node* node){
		write(fd, (void*)node, sizeof(*node));
	}

	Node* TreeSerializer::readNode(){
		Node* node = (Node*)malloc(sizeof(Node));
		read(fd, (void*)node, sizeof(Node));
		return node;
	}
int main(int argc, char** argv){
	TreeSerializer TS("test", MODE::WRITE);
	Node* node = new Node(8);
    dummy_node d_node = dummy_node{.x = 1, .y = 2, .z = 3};
	node->key = 12;
	TS.writeNode(&d_node);
	dummy_node* second_node = TS.readNode();
	std::cout << second_node->x;
	return 0;
}
