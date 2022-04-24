#include "Tree.hpp"
#include "algorithm.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

#pragma once

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "[ERROR]: Usage is ./treefile <filepath> <num_elements>"
              << std::endl;
    exit(1);
  }
  long num_elements = std::stol(argv[2]);
  Tree tree(2);
  std::vector<int> keys, values;
  tree.fill(num_elements, 1000, keys, values);

  // tree.init_serializer(argv[1]);
  // tree.dump_tree();
  // auto maybe_head_ptr = tree.digestNode();
  // std::cout << (maybe_head_ptr->id == tree.get_head_ref()->key) << std::endl;

  Node *root = tree.get_head_ref();

  auto begin = std::chrono::high_resolution_clock::now();
  s_bfs(root, 4);
  auto end = std::chrono::high_resolution_clock::now();
  auto ms_int =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  std::cout << "Serial BFS " << ms_int.count() << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  s_dfs(root, 4);
  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  std::cout << "Serial DFS " << ms_int.count() << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  p_sche(root, 0, 4);

  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  std::cout << "Parallel BFS " << ms_int.count() << std::endl;

  begin = std::chrono::high_resolution_clock::now();

  p_sche(root, 1, 4);

  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  std::cout << "Parallel DFS " << ms_int.count() << std::endl;

  return 0;
}
