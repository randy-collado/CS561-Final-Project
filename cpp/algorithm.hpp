#include "Tree.hpp"

#include <omp.h>
#include <queue>
#include <vector>
// #include <future>
// #include <thread>

// void p_dfs(S_Node *curNode, int key, bool *isFound, std::queue<Node *>
// *nodeQ); void p_bfs(S_Node *curNode, int key, bool *isFound, std::queue<Node
// *> *nodeQ); void p_sche(S_Node *root, int type, int key);
bool s_bfs(Tree *tree, int key);
bool s_dfs(Tree *tree, int offset, int key);
bool s_iddfs_worker(Tree *tree, int key, int offset, size_t depLeft);
bool s_iddfs(Tree *tree, int key, size_t maxDepth);

bool p_bfs_omp(Tree *tree, int key);
bool p_dfs_omp(Tree *tree, int offset, int key);
bool p_iddfs_omp(Tree *tree, int key, size_t maxDepth);
bool p_iddfs_worker(Tree *tree, int offset, int key, size_t depLeft);

bool p_mixed_omp(Tree *tree, int offset, int key);
