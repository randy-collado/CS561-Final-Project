#include <iostream>
#include <vector>
#include "Tree.hpp"


int main(int argc, char** argv){
	if (argc < 3) {
		std::cerr << "[ERROR]: Usage is ./treefile <filepath> <num_elements>" << std::endl; 
		exit(1);
	}
	long num_elements = std::stol(argv[2]);
	Tree tree(2);
	std::vector<int> keys, values;
	tree.fill(20, 20, keys, values);
	tree.BFS(keys[0]);
	tree.init_serializer(argv[1]);
	tree.dump_tree_tiered();
	auto maybe_metadata = tree.digest_metadata();
    for (size_t i = 0; i < maybe_metadata.tiers; ++i){
        std::cout << maybe_metadata.offsets[i] << std::endl;
    }
	return 0;
}
