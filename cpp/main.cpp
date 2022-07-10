#include "Tree.hpp"
#include "algorithm.hpp"
#include <chrono>
#include <cstring>
#include <ctime>
#include <iostream>
#include <math.h>
#include <sched.h>
#include <vector>

int64_t timeAndPrint(Tree *tree, int targetKey, bool (*func)(Tree *, int &),
                     std::string funcName) {
  auto begin = std::chrono::high_resolution_clock::now();
  bool res = func(tree, targetKey);
  auto end = std::chrono::high_resolution_clock::now();
  auto ms_int =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  printf("%s: %lld ms, result: %d\n", funcName.data(), ms_int.count(), res);

  return ms_int.count();
}

int64_t testPDFS(Tree *tree, int targetKey, int fSize) {
  auto begin = std::chrono::high_resolution_clock::now();
  auto res = p_dfs_omp(tree, targetKey, fSize);
  auto end = std::chrono::high_resolution_clock::now();
  auto ms_int =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  printf("PDFS (Omp), fSize %d: %lld ms, result: %d\n", fSize, ms_int.count(),
         res);
  return ms_int.count();
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "[ERROR]: Usage is ./main <data_path>" << std::endl;
    exit(1);
  }

  std::cout << "Cancellation Env: " << omp_get_cancellation() << std::endl;
  Tree tree(0);
  tree.init_serializer(argv[1], 0);
  tree.init_metadata();

  // Test 1: direct IO + tuning fSize in parallel DFS
  printf("==============Test 1: Not Exist Key + Variable Thread "
         "Number============\n");
  int targetKey = -1;
  printf("Target Key: %d\n", targetKey);

  auto begin = std::chrono::high_resolution_clock::now();
  auto res = s_dfs(&tree, 0, targetKey);
  auto end = std::chrono::high_resolution_clock::now();
  auto ms_int =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  printf("Serial DFS: %lld ms, result: %d.\n", ms_int.count(), res);

  timeAndPrint(&tree, targetKey, s_dfs_2, "Serial DFS (No recursive)");

  timeAndPrint(&tree, targetKey, s_bfs, "Serial BFS");

  std::vector<int> cpus = {1, 2, 4, 6, 8};
  std::vector<int> threads = {1, 2, 4, 8, 16, 32, 64, 80, 100, 120};
  for (auto i = 0; i < threads.size(); i++) {

    printf("-----------Thread Number: %d--------------\n", threads.at(i));

    omp_set_num_threads(threads.at(i));

    timeAndPrint(&tree, targetKey, p_bfs_omp, "Parallel BFS (Omp)");

    // timeAndPrint(&tree, targetKey, p_dfs_omp, "Parallel DFS (Omp)");
    begin = std::chrono::high_resolution_clock::now();
    std::cout << p_dfs_omp(&tree, targetKey, 8) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    printf("Parallel DFS (Omp): %lld ms.\n", ms_int.count());
  }

  // Test 2: Algorithm Performance
  printf("==============Test 2: Random Key + Variable fSize============\n");
  int64_t total_sbfs = 0;
  int64_t total_sdfs = 0;
  int64_t total_pbfs = 0;

  int numfSize = 10;
  std::vector<int> fSizeList = {1, 2, 4, 6, 8, 10, 12, 14, 16, 20};
  std::vector<int64_t> total_pdfs;
  total_pdfs.reserve(numfSize);

  int numTests = 20;

  for (int i = 0; i < numTests + 1; i++) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, tree.numNode);

    int targetKey = (i == numTests) ? -1 : distrib(gen);

    std::cout << "============ Round " << i + 1 << " Target Key " << targetKey
              << " ============" << std::endl;
    auto begin = std::chrono::high_resolution_clock::now();
    auto res = s_dfs(&tree, 0, targetKey);
    auto end = std::chrono::high_resolution_clock::now();
    auto ms_int =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    printf("Serial DFS: %lld ms, result: %d\n", ms_int.count(), res);

    total_sdfs +=
        timeAndPrint(&tree, targetKey, s_dfs_2, "Serial DFS (No recursive)");

    total_sbfs += timeAndPrint(&tree, targetKey, s_bfs, "Serial BFS");

    omp_set_num_threads(32);

    total_pbfs +=
        timeAndPrint(&tree, targetKey, p_bfs_omp, "Parallel BFS (Omp)");

    for (int i = 0; i < numfSize; i++) {
      total_pdfs[i] += testPDFS(&tree, targetKey, fSizeList[i]);
    }
  }

  printf("=================== FINAL RESULT =================\n");
  std::cout << "Cancellation Env: " << omp_get_cancellation() << std::endl;
  printf("Serial BFS : %lld ms.\n", total_sbfs);
  printf("Serial DFS : %lld ms.\n", total_sdfs);
  printf("Parallel BFS : %lld ms.\n", total_pbfs);

  for (int i = 0; i < numfSize; i++) {
    printf("PDFS (Omp), fSize %d: %lld ms.\n", fSizeList[i], total_pdfs[i]);
  }

  return 0;
}
