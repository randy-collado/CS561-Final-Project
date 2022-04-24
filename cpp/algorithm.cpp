#include "algorithm.hpp"

bool s_bfs(Node *root, int key) {
  std::queue<Node *> nodeQ;
  nodeQ.push(root);
  while (!nodeQ.empty()) {
    Node *curNode = nodeQ.front();
    nodeQ.pop();
    if (curNode->key == key)
      return true;
    for (int i = 0; i < curNode->numChildren; i++) {
      nodeQ.push(curNode->children[i]);
    }
  }
  return false;
}

bool s_dfs(Node *curNode, int key) {
  if (curNode->key == key)
    return true;
  for (int i = 0; i < curNode->numChildren; i++) {
    if (s_dfs(curNode->children[i], key))
      return true;
  }
  return false;
}

bool s_iddfs_worker(Node *curNode, int key, int depLeft) {
  if (curNode->key == key)
    return true;
  if (depLeft == 0)
    return false;
  for (int i = 0; i < curNode->numChildren; i++) {
    if (s_iddfs_worker(curNode->children[i], key, depLeft - 1))
      return true;
  }
  return false;
}

bool s_iddfs(Node *root, int key) {
  int dep = 1;
  while (1) {
    if (s_iddfs_worker(root, key, dep))
      return true;
    dep++;
  }
  return false;
}

void p_dfs(Node *curNode, int key, bool *isFound, std::queue<Node *> *nodeQ) {
  while (1) {
    // std::cout << curNode << " " << curNode->key << std::endl;
    if (*isFound)
      return;
    if (curNode->key == key) {
      *isFound = true;
      return;
    }
    for (int i = 1; i < curNode->numChildren; i++) {
      (*nodeQ).push(curNode->children[i]);
    }
    if (curNode->numChildren >= 1) {
      curNode = curNode->children[0];
    } else
      break;
  }
}

void p_bfs(Node *curNode, int key, bool *isFound, std::queue<Node *> *nodeQ) {
  // std::cout << curNode << " " << curNode->key << std::endl;
  if (*isFound)
    return;
  if (curNode->key == key) {
    *isFound = true;
    return;
  }
  for (int i = 0; i < curNode->numChildren; i++) {
    (*nodeQ).push(curNode->children[i]);
  }
}

void p_sche(Node *root, int type, int key) {
  bool isFound = false;
  std::queue<Node *> nodeQ;
  nodeQ.push(root);
  while (1) {
    if (isFound)
      break;
    if (!nodeQ.empty()) {
      Node *t = nodeQ.front();
      nodeQ.pop();
      if (type == 0) {
        std::async(p_bfs, t, key, &isFound, &nodeQ);
      } else if (type == 1) {
        std::async(p_dfs, t, key, &isFound, &nodeQ);
      }
    }
  }
}

bool p_bfs_omp(Node *root, int key) {
  std::queue<Node *> nodeQ;
  nodeQ.push(root);
  std::cout << omp_get_max_threads() << std::endl;
  while (!nodeQ.empty()) {
    Node *curNode = nodeQ.front();
    nodeQ.pop();
    if (curNode->key == key)
      return true;

#pragma omp parallel for

    for (int i = 0; i < curNode->numChildren; i++) {
      nodeQ.push(curNode->children[i]);
    }
  }
  return false;
}

bool p_dfs_omp(Node *curNode, int key) {
  if (curNode->key == key)
    return true;

  bool isFound = false;

#pragma omp parallel for
  for (int i = 0; i < curNode->numChildren; i++) {
    // std::cout << omp_get_thread_num() << std::endl;
    if (!isFound && p_dfs_omp(curNode->children[i], key))
      isFound = true;
  }
  return isFound;
}
