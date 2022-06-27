#include "Tree.hpp"
#include "algorithm.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 4) {
    std::cerr << "[ERROR]: Usage is ./generator <data_path> <num_elements> <branch> "
              << std::endl;
    exit(1);
  }
  int num_elements = std::stol(argv[2]);
  printf("# of elements: %d\n", num_elements);

  int max_branch = std::stoi(argv[3]);

  printf("Branch size: %d\n", max_branch);

  Tree tree(max_branch);
  std::vector<int> keys, values;

  tree.seq_fill(num_elements, keys, values);

  tree.init_serializer(argv[1], 1);

  tree.dump_tree();

  printf("Node Count: %lld\n", tree.nodeCount);

  return 0;
}
