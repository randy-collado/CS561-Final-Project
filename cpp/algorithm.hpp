#include "Tree.hpp"

#include <omp.h>
#include <queue>
#include <vector>
// #include <future>
// #include <thread>

// void p_dfs(Node *curNode, int key, bool *isFound, std::queue<Node *> *nodeQ);
// void p_bfs(Node *curNode, int key, bool *isFound, std::queue<Node *> *nodeQ);
// void p_sche(Node *root, int type, int key);
bool s_bfs(Node *root, int key);
bool s_dfs(Node *curNode, int key);
bool s_iddfs_worker(Node *curNode, int key, size_t depLeft);
bool s_iddfs(Node *root, int key, size_t maxDepth);

bool p_bfs_omp(Node *root, int key);
bool p_dfs_omp(Node *curNode, int key);
