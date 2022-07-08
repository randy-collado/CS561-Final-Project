#include "Tree.hpp"

#include <omp.h>
#include <queue>
#include <vector>
// #include <future>
// #include <thread>

// void p_dfs(S_Node *curNode, int key, bool *isFound, std::queue<TreeNode *>
// *nodeQ); void p_bfs(S_Node *curNode, int key, bool *isFound, std::queue<TreeNode
// *> *nodeQ); void p_sche(S_Node *root, int type, int key);
bool s_bfs(Tree *tree, int &key);
bool s_dfs(Tree *tree, int number, int &key);
bool s_dfs_2(Tree *tree, int &key);

bool s_iddfs_worker(Tree *tree, int number, int &key, size_t depLeft);
bool s_iddfs(Tree *tree, int &key, size_t maxDepth);

bool p_bfs_omp(Tree *tree, int &key);
// bool p_dfs_omp(Tree *tree, int number, int &key);
bool p_iddfs_omp(Tree *tree, int &key, size_t maxDepth);
bool p_iddfs_worker(Tree *tree, int number, int &key, size_t depLeft);

bool p_hybrid_omp(Tree *tree, int number, int &key, int &brhThres);

bool p_test_omp(Tree *tree, int number, int &key, int &maxFSize);

bool _p_dfs_omp(Tree *tree, int &key, std::vector<int> &frontier, int &fSize);
bool p_dfs_omp(Tree *tree, int &key, int f_size);
