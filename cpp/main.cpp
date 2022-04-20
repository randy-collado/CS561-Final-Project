#include <iostream>
#include <vector>
#include "Tree.hpp"
#include "algorithm.hpp"


int main(int argc, char** argv){
	if (argc < 3) {
		std::cerr << "[ERROR]: Usage is ./treefile <filepath> <num_elements>" << std::endl; 
		exit(1);
	}
	long num_elements = std::stol(argv[2]);
	Tree tree(2);
	std::vector<int> keys, values;
	tree.fill(20, 20, keys, values);
	
	// tree.init_serializer(argv[1]);
	// tree.dump_tree();
	// auto maybe_head_ptr = tree.digestNode();
	// std::cout << (maybe_head_ptr->id == tree.get_head_ref()->key) << std::endl;

	Node* root = tree.get_head_ref();
	p_sche(root, 0, 4);

	std::cout << "--------" << std::endl;
	p_sche(root, 1, 4);

	return 0;
}
