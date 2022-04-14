#include "Tree.hpp"
#include "TreeSerializer.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#define align(p, a) (((long)(p) + (a - 1)) & ~(a - 1))

	/*
	 * NOTE: The size of a page on this device is 4096 bytes
	 */	
	TreeSerializer::TreeSerializer() : at_eof(false), fd(-1), mode_internal(MODE::INVALID), read_offset(0), offset(0) {}

	TreeSerializer::~TreeSerializer(){
		close(fd);
	}

	void TreeSerializer::openFile(std::string filepath, MODE mode){
	int flags = O_DIRECT | O_SYNC;
			switch(mode){
				case MODE::READ: //READ
					flags |= O_RDONLY;
					this->mode_internal = mode;
					break;
				case MODE::WRITE: //WRITE
					flags |= O_RDWR | O_CREAT;
					this->mode_internal = mode;
					break;
				default: break;
			}
			this->fd = open(filepath.c_str(), O_RDWR | O_CREAT);
			if (this->fd < 0) {
				std::cerr << "[ERROR]: Serializer failed\n[MESSAGE]: file unable to be opened or created";
				exit(1);
			}
	}

	void TreeSerializer::writeNode(S_Node* node){
		ssize_t bytes = pwrite(this->fd, (char*)node, sizeof(S_Node), this->offset);
		std::cout << "Bytes written: " << bytes << std::endl;
		if (bytes <= 0) { 
			std::cout << "Could not write\nReason: " + std::string(strerror(errno))<<std::endl;
			return;
		}
		this->offset += bytes;
	}

	S_Node* TreeSerializer::readNode(){
		S_Node* node = new S_Node();
		ssize_t bytes = pread(this->fd, (char*)node, sizeof(S_Node), read_offset);
		std::cout << "read node " << node->id << std::endl;
		std::cout << "Bytes read: " << bytes << std::endl;
		if(bytes<=0) std::cout << "Could not Read\nReason: " + std::string(strerror(errno))<<std::endl;
		return node;
	}

