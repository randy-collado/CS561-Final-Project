#include "Tree.hpp"
#include "algorithm.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>
#include <sched.h>
#include <math.h>

int main(int argc, char **argv) {
  if (argc < 4) {
    std::cerr << "[ERROR]: Usage is ./main <filepath> <targetKey> <numNode>"
              << std::endl;
    exit(1);
  }

  Tree tree(8);
  tree.init_serializer(argv[1]);

  // Test 1: direct IO + tuning fSize in parallel DFS
  printf("==============Test 1: Not Exist Key + Variable Thread Number============\n");
  int targetKey = -1;
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

  std::vector<int> cpus = {1, 2, 4, 6, 8};
  std::vector<int> threads = {1, 2, 4, 8, 16, 32, 64, 80, 100, 120};
  for (int i = 0; i < threads.size(); i ++) {

    printf("-----------Thread Number: %d--------------\n", threads.at(i));

    omp_set_num_threads(threads.at(i));

    begin = std::chrono::high_resolution_clock::now();
    std::cout << p_bfs_omp(&tree, targetKey) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    printf("Parallel BFS (Omp): %lld ms.\n", ms_int.count());

    begin = std::chrono::high_resolution_clock::now();
    std::cout << p_dfs_omp(&tree, targetKey, 8) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    printf("Parallel DFS (Omp): %lld ms.\n", ms_int.count());

  }


  // Test 2: Algorithm Efficiency
  printf("==============Test 2: Random Key + Variable fSize============\n");
  auto total_sbfs = 0;
  auto total_sdfs = 0;
  auto total_pbfs = 0;
  auto total_pdfs1 = 0;
  auto total_pdfs2 = 0;
  auto total_pdfs4 = 0;
  auto total_pdfs6 = 0;
  auto total_pdfs8 = 0;
  auto total_pdfs10 = 0;
  auto total_pdfs12 = 0;
  auto total_pdfs14 = 0;
  auto total_pdfs16 = 0;
  auto total_pdfs20 = 0;


  for (int i=0; i<21; i++){
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> distrib(0, std::stoi(argv[3]));

      int targetKey = distrib(gen);
      if (i==20){targetKey = -1;}
      std::cout << "============ Round " << i+1 << " Target Key " << targetKey << " ============" << std::endl;
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
      total_sdfs += ms_int.count();
      printf("Serial DFS (No recursive): %lld ms.\n", ms_int.count());

      begin = std::chrono::high_resolution_clock::now();
      std::cout << s_bfs(&tree, targetKey) << std::endl;
      end = std::chrono::high_resolution_clock::now();
      ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      total_sbfs += ms_int.count();
      printf("Serial BFS: %lld ms.\n", ms_int.count());

      omp_set_num_threads(32);

      begin = std::chrono::high_resolution_clock::now();
      std::cout << p_bfs_omp(&tree, targetKey) << std::endl;
      end = std::chrono::high_resolution_clock::now();
      ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      total_pbfs += ms_int.count();
      printf("Parallel BFS (Omp): %lld ms.\n", ms_int.count());

      
      begin = std::chrono::high_resolution_clock::now();
      std::cout << p_dfs_omp(&tree, targetKey, 1) << std::endl;
      end = std::chrono::high_resolution_clock::now();
      ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      total_pdfs1 += ms_int.count();
      printf("Parallel DFS (Omp) with fSize 1: %lld ms.\n", ms_int.count());

      begin = std::chrono::high_resolution_clock::now();
      std::cout << p_dfs_omp(&tree, targetKey, 2) << std::endl;
      end = std::chrono::high_resolution_clock::now();
      ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      total_pdfs2 += ms_int.count();
      printf("Parallel DFS (Omp) with fSize 2: %lld ms.\n", ms_int.count());

      begin = std::chrono::high_resolution_clock::now();
      std::cout << p_dfs_omp(&tree, targetKey, 4) << std::endl;
      end = std::chrono::high_resolution_clock::now();
      ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      total_pdfs4 += ms_int.count();
      printf("Parallel DFS (Omp) with fSize 4: %lld ms.\n", ms_int.count());

      begin = std::chrono::high_resolution_clock::now();
      std::cout << p_dfs_omp(&tree, targetKey, 6) << std::endl;
      end = std::chrono::high_resolution_clock::now();
      ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      total_pdfs6 += ms_int.count();
      printf("Parallel DFS (Omp) with fSize 6: %lld ms.\n", ms_int.count());

      begin = std::chrono::high_resolution_clock::now();
      std::cout << p_dfs_omp(&tree, targetKey, 8) << std::endl;
      end = std::chrono::high_resolution_clock::now();
      ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      total_pdfs8 += ms_int.count();
      printf("Parallel DFS (Omp) with fSize 8: %lld ms.\n", ms_int.count());

      begin = std::chrono::high_resolution_clock::now();
      std::cout << p_dfs_omp(&tree, targetKey, 10) << std::endl;
      end = std::chrono::high_resolution_clock::now();
      ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      total_pdfs10 += ms_int.count();
      printf("Parallel DFS (Omp) with fSize 10: %lld ms.\n", ms_int.count());

      begin = std::chrono::high_resolution_clock::now();
      std::cout << p_dfs_omp(&tree, targetKey, 12) << std::endl;
      end = std::chrono::high_resolution_clock::now();
      ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      total_pdfs12 += ms_int.count();
      printf("Parallel DFS (Omp) with fSize 12: %lld ms.\n", ms_int.count());

      begin = std::chrono::high_resolution_clock::now();
      std::cout << p_dfs_omp(&tree, targetKey, 14) << std::endl;
      end = std::chrono::high_resolution_clock::now();
      ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      total_pdfs14 += ms_int.count();
      printf("Parallel DFS (Omp) with fSize 14: %lld ms.\n", ms_int.count());

      begin = std::chrono::high_resolution_clock::now();
      std::cout << p_dfs_omp(&tree, targetKey, 16) << std::endl;
      end = std::chrono::high_resolution_clock::now();
      ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      total_pdfs16 += ms_int.count();
      printf("Parallel DFS (Omp) with fSize 16: %lld ms.\n", ms_int.count());

      begin = std::chrono::high_resolution_clock::now();
      std::cout << p_dfs_omp(&tree, targetKey, 20) << std::endl;
      end = std::chrono::high_resolution_clock::now();
      ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
      total_pdfs20 += ms_int.count();
      printf("Parallel DFS (Omp) with fSize 20: %lld ms.\n", ms_int.count());
  }

  printf("=================== FINAL RESULT =================\n");
  printf("Serial BFS : %lld ms.\n", total_sbfs);
  printf("Serial DFS : %lld ms.\n", total_sdfs);
  printf("Parallel BFS : %lld ms.\n", total_pbfs);
  printf("Parallel DFS (Omp) with fSize 1: %lld ms.\n", total_pdfs1);
  printf("Parallel DFS (Omp) with fSize 2: %lld ms.\n", total_pdfs2);
  printf("Parallel DFS (Omp) with fSize 4: %lld ms.\n", total_pdfs4);
  printf("Parallel DFS (Omp) with fSize 6: %lld ms.\n", total_pdfs6);
  printf("Parallel DFS (Omp) with fSize 8: %lld ms.\n", total_pdfs8);
  printf("Parallel DFS (Omp) with fSize 10: %lld ms.\n", total_pdfs10);
  printf("Parallel DFS (Omp) with fSize 12: %lld ms.\n", total_pdfs12);
  printf("Parallel DFS (Omp) with fSize 14: %lld ms.\n", total_pdfs14);
  printf("Parallel DFS (Omp) with fSize 16: %lld ms.\n", total_pdfs16);
  printf("Parallel DFS (Omp) with fSize 20: %lld ms.\n", total_pdfs20);

  return 0;
}
