#include "Tree.hpp"
#include <future>
#include <queue>
#include <thread>
void p_dfs(Node *curNode, int key, bool *isFound, std::queue<Node *> *nodeQ);
void p_bfs(Node *curNode, int key, bool *isFound, std::queue<Node *> *nodeQ);
void p_sche(Node *root, int type, int key);
bool s_bfs(Node *root, int key);
bool s_dfs(Node *curNode, int key);
