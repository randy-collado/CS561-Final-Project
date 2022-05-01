#include "Tree.hpp"
#include "algorithm.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "[ERROR]: Usage is ./treefile <filepath> <num_elements> "
                 "(<max_key> <max_branch>)"
              << std::endl;
    exit(1);
  }
  int num_elements = std::stol(argv[2]);
  printf("# of elements: %d\n", num_elements);

  int max_branch = 2;
  if (argc >= 5)
    max_branch = std::stoi(argv[4]);
  if (max_branch > 8)
    max_branch = 8;

  printf("Branch size: %d\n", max_branch);

  Tree tree(max_branch);
  std::vector<int> keys, values;

  int maxKey = num_elements;
  if (argc >= 4)
    maxKey = std::stoi(argv[3]);

  printf("Maximum key: %d\n", maxKey);

  tree.fill(num_elements, maxKey, keys, values);

  tree.init_serializer(argv[1]);
  tree.dump_tree();

  // auto maybe_metadata = tree.digest_metadata();
  // for (size_t i = 0; i < maybe_metadata.tiers; ++i) {
  //   std::cout << maybe_metadata.offsets[i] << std::endl;
  // }

  // return 0;

  // S_Node *root = tree.read_from_TS(0);

  printf("S_Node size: %lld\n", sizeof(S_Node));
  printf("Node Count: %lld\n", tree.nodeCount);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, maxKey);

  int targetKey = distrib(gen);
  printf("Target Key: %d\n", targetKey);

  auto begin = std::chrono::high_resolution_clock::now();
  std::cout << s_dfs(&tree, 0, targetKey) << std::endl;
  auto end = std::chrono::high_resolution_clock::now();
  auto ms_int =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  printf("Serial DFS: %lld ms.\n", ms_int.count());

  begin = std::chrono::high_resolution_clock::now();
  std::cout << s_dfs_2(&tree, targetKey) << std::endl;
  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  printf("Serial DFS (No recursive): %lld ms.\n", ms_int.count());

  begin = std::chrono::high_resolution_clock::now();
  std::cout << s_bfs(&tree, targetKey) << std::endl;
  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  printf("Serial BFS: %lld ms.\n", ms_int.count());

  begin = std::chrono::high_resolution_clock::now();
  std::cout << s_iddfs(&tree, targetKey, tree.get_max_level()) << std::endl;
  end = std::chrono::high_resolution_clock::now();
  ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  printf("Serial IDDFS: %lld ms.\n", ms_int.count());

  for (int i = 1; i <= 256; i *= 2) {

    omp_set_num_threads(i);
    printf("Thread Count: %d\n", i);

    begin = std::chrono::high_resolution_clock::now();
    std::cout << p_bfs_omp(&tree, targetKey) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    printf("Parallel BFS (Omp): %lld ms.\n", ms_int.count());

    begin = std::chrono::high_resolution_clock::now();
    std::cout << p_dfs_omp(&tree, targetKey) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    printf("Parallel DFS (Omp): %lld ms.\n", ms_int.count());

    begin = std::chrono::high_resolution_clock::now();
    std::cout << p_iddfs_omp(&tree, targetKey, tree.get_max_level())
              << std::endl;
    end = std::chrono::high_resolution_clock::now();
    ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    printf("Parallel IDDFS (Omp): %lld ms.\n", ms_int.count());

    int brhThres = 3;

    begin = std::chrono::high_resolution_clock::now();
    std::cout << p_hybrid_omp(&tree, 0, targetKey, brhThres) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    printf("Parallel Mixed (Omp): %lld ms.\n", ms_int.count());
  }

  return 0;
}
