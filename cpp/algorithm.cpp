#include "algorithm.hpp"

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
