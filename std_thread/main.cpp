#include <windows.h>

#include <chrono>
#include <cstring>
#include <ctime>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include "writeTree.h"

using namespace std;

bool dfs(Node *curNode, int key) {
  // std::cout << curNode->key << std::endl;
  if (curNode->key == key)
    return 1;
  for (int i = 0; i < curNode->chs.size(); i++) {
    if (dfs(curNode->chs[i], key)) {
      return 1;
    }
  }
  return 0;
}

bool isFound = false;

void p_dfs(Node *curNode, int key) {
  while (1) {
    if (isFound)
      return;
    if (curNode->key == key) {
      isFound = true;
      return;
    }
    for (int i = 1; i < curNode->chs.size(); i++) {
      // thread(p_dfs, curNode->chs[i], key);
      auto tmp = async(p_dfs, curNode->chs[i], key);
    }
    if (curNode->chs.size() >= 1) {
      curNode = curNode->chs[0];
    } else
      break;
  }
}

int cntNode = 0;

void gen_tree(Node *root, int dep) {
  if (dep == 0)
    return;
  for (int i = 0; i < rand() % 10; i++) {
    Node *newNode = new Node();
    // newNode->chs = nullptr;
    newNode->key = ++cntNode;
    root->chs.push_back(newNode);
    gen_tree(newNode, dep - 1);
  }
}

int main() {
  srand(time(0));
  Node *root = new Node();
  // root->chs = nullptr;
  root->key = -1;
  gen_tree(root, 4);
  writeTreeToFile("test", root);
  // cout << "Node count: " << cntNode << endl;
  // cout << dfs(root, cntNode) << endl;
  // p_dfs(root, cntNode);
  return 0;
}