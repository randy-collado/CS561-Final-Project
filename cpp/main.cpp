#include "Tree.hpp"
#include "algorithm.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "[ERROR]: Usage is ./treefile <filepath> <num_elements>"
              << std::endl;
    exit(1);
  }
  long num_elements = std::stol(argv[2]);
  Tree tree(2);
  std::vector<int> keys, values;

  tree.fill(num_elements, num_elements / 5, keys, values);

  tree.init_serializer(argv[1]);
  tree.dump_tree_tiered();
  auto maybe_metadata = tree.digest_metadata();
  for (size_t i = 0; i < maybe_metadata.tiers; ++i) {
    std::cout << maybe_metadata.offsets[i] << std::endl;
  }

  Node *root = tree.get_head_ref();

  int targetKey = rand() % num_elements;

  printf("Target Key: %d\n", targetKey);

  auto begin = std::chrono::high_resolution_clock::now();
  s_bfs(root, targetKey);
  auto end = std::chrono::high_resolution_clock::now();
  auto ms_int =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  printf("Serial BFS: %lld ns.\n", ms_int.count());

  begin = std::chrono::high_resolution_clock::now();
  s_dfs(root, targetKey);
  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  printf("Serial DFS: %lld ns.\n", ms_int.count());

  begin = std::chrono::high_resolution_clock::now();
  s_iddfs(root, targetKey, tree.get_max_level());
  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  printf("Serial IDDFS: %lld ns.\n", ms_int.count());

  // begin = std::chrono::high_resolution_clock::now();
  // p_sche(root, 0, 4);

  // end = std::chrono::high_resolution_clock::now();
  // ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  // std::cout << "Parallel BFS " << ms_int.count() << std::endl;

  // begin = std::chrono::high_resolution_clock::now();

  // p_sche(root, 1, 4);

  // end = std::chrono::high_resolution_clock::now();
  // ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  // std::cout << "Parallel DFS " << ms_int.count() << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  p_bfs_omp(root, targetKey);
  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  printf("Parallel BFS (Omp): %lld ns.\n", ms_int.count());

  begin = std::chrono::high_resolution_clock::now();
  p_dfs_omp(root, targetKey);
  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  printf("Parallel DFS (Omp): %lld ns.\n", ms_int.count());

  return 0;
}
