#include "Tree.hpp"
#include "algorithm.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 4) {
    std::cerr
        << "[ERROR]: Usage is ./treefile <filepath> <num_elements> <max_key>"
        << std::endl;
    exit(1);
  }
  int num_elements = std::stol(argv[2]);
  Tree tree(2);
  std::vector<int> keys, values;
  int maxKey = std::stoi(argv[3]);

  tree.fill(num_elements, maxKey, keys, values);

  tree.init_serializer(argv[1]);
  tree.dump_tree();
  // auto maybe_metadata = tree.digest_metadata();
  // for (size_t i = 0; i < maybe_metadata.tiers; ++i) {
  //   std::cout << maybe_metadata.offsets[i] << std::endl;
  // }

  // return 0;

  // S_Node *root = tree.read_from_TS(0);

  printf("S_Node size: %d\n", sizeof(S_Node));

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, maxKey);

  int targetKey = distrib(gen);
  printf("Target Key: %d\n", targetKey);

  auto begin = std::chrono::high_resolution_clock::now();
  std::cout << s_dfs(&tree, 0, targetKey) << std::endl;
  auto end = std::chrono::high_resolution_clock::now();
  auto ms_int =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  printf("Serial DFS: %lld ns.\n", ms_int.count());

  begin = std::chrono::high_resolution_clock::now();
  std::cout << s_bfs(&tree, targetKey) << std::endl;
  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  printf("Serial BFS: %lld ns.\n", ms_int.count());

  begin = std::chrono::high_resolution_clock::now();
  std::cout << s_iddfs(&tree, targetKey, tree.get_max_level()) << std::endl;
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
  std::cout << p_bfs_omp(&tree, targetKey) << std::endl;
  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  printf("Parallel BFS (Omp): %lld ns.\n", ms_int.count());

  begin = std::chrono::high_resolution_clock::now();
  std::cout << p_dfs_omp(&tree, 0, targetKey) << std::endl;
  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  printf("Parallel DFS (Omp): %lld ns.\n", ms_int.count());

  return 0;
}
