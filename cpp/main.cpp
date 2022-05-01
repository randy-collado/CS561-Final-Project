#include "Tree.hpp"
#include "algorithm.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>
#include <sched.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "[ERROR]: Usage is ./main <filepath> <targetKey>"
              << std::endl;
    exit(1);
  }

  Tree tree(8);
  tree.init_serializer(argv[1]);

  int targetKey = std::stoi(argv[2]);
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

  // begin = std::chrono::high_resolution_clock::now();
  // std::cout << s_iddfs(&tree, targetKey, tree.get_max_level()) << std::endl;
  // end = std::chrono::high_resolution_clock::now();
  // ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  // printf("Serial IDDFS: %lld ms.\n", ms_int.count());

  std::vector<int> cpus = {1, 2, 4, 6, 8};
  std::vector<int> threads = {1, 2, 4, 8, 16, 32, 64, 80, 100, 120};

  for (int j=0; j < cpus.size(); j++){
      cpu_set_t cpu_set;
      CPU_ZERO(&cpu_set);
      for (int cpu=0; cpu<cpus.at(j); cpu++){
        CPU_SET(cpu, &cpu_set);
      }
      if(sched_setaffinity(0, sizeof(cpu_set), &cpu_set) < 0)
            perror("sched_setaffinity");
      printf("+++++++++++++++++++++++++++++++++++++\n");
      printf("CPU Number: %d\n", cpus.at(j));

      for (int i = 0; i < threads.size(); i ++) {

        printf("-----------Chaning Num Threads to %d--------------\n", threads.at(i));

        omp_set_num_threads(threads.at(i));

        begin = std::chrono::high_resolution_clock::now();
        std::cout << p_bfs_omp(&tree, targetKey) << std::endl;
        end = std::chrono::high_resolution_clock::now();
        ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        printf("Parallel BFS (Omp): %lld ms.\n", ms_int.count());

        // begin = std::chrono::high_resolution_clock::now();
        // std::cout << p_dfs_omp(&tree, targetKey) << std::endl;
        // end = std::chrono::high_resolution_clock::now();
        // ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        // printf("Parallel DFS (Omp): %lld ms.\n", ms_int.count());

        // begin = std::chrono::high_resolution_clock::now();
        // std::cout << p_iddfs_omp(&tree, targetKey, tree.get_max_level())
        //           << std::endl;
        // end = std::chrono::high_resolution_clock::now();
        // ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        // printf("Parallel IDDFS (Omp): %lld ms.\n", ms_int.count());

        // int brhThres = 3;

        // begin = std::chrono::high_resolution_clock::now();
        // std::cout << p_hybrid_omp(&tree, 0, targetKey, brhThres) << std::endl;
        // end = std::chrono::high_resolution_clock::now();
        // ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        // printf("Parallel Mixed (Omp): %lld ms.\n", ms_int.count());
      }
  }
  return 0;
}
