#include "algorithm.hpp"

bool s_bfs(Tree *tree, int key) {
  std::vector<int> frontier;
  std::vector<int> next;

  frontier.push_back(0);

  bool isFound = false;
  int rfCnt = 0;

  while (!isFound && frontier.size() > 0) {
    for (size_t i = 0; i < frontier.size(); i++) {
      S_Node *s_node = tree->read_from_TS(frontier[i]);
      rfCnt++;
      if (s_node->key == key)
        isFound = true;
      if (isFound)
        continue;
      std::vector<int> next_private(s_node->children,
                                    s_node->children + s_node->numChildren);
      next.insert(next.end(), next_private.begin(), next_private.end());
    }
    frontier = next;
    next.clear();
  }
  printf("Read file count = %d\n", rfCnt);
  return isFound;
  // std::deque<int> offsetQ;
  // offsetQ.push_back(0);

  // while (!offsetQ.empty()) {
  //   // printf("%d\n", offsetQ.front());
  //   S_Node *s_node = tree->read_from_TS(offsetQ.front());
  //   // std::cout << s_node << std::endl;
  //   offsetQ.pop_front();
  //   if (s_node->key == key)
  //     return true;
  //   for (int i = 0; i < s_node->numChildren; i++) {
  //     offsetQ.insert(offsetQ.end(), s_node->children,
  //                    s_node->children + s_node->numChildren);
  //   }
  // }
  // return false;
}

bool s_dfs(Tree *tree, int offset, int key) {
  S_Node *s_node = tree->read_from_TS(offset);
  if (s_node->key == key)
    return true;
  for (int i = 0; i < s_node->numChildren; i++) {
    if (s_dfs(tree, s_node->children[i], key))
      return true;
  }
  return false;
}

bool s_iddfs_worker(Tree *tree, int offset, int key, size_t depLeft) {
  S_Node *s_node = tree->read_from_TS(offset);
  if (s_node->key == key)
    return true;
  if (depLeft == 0)
    return false;
  for (int i = 0; i < s_node->numChildren; i++) {
    if (s_iddfs_worker(tree, s_node->children[i], key, depLeft - 1))
      return true;
  }
  return false;
}

bool s_iddfs(Tree *tree, int key, size_t maxDepth) {
  size_t dep = 1;
  while (dep <= maxDepth) {
    if (s_iddfs_worker(tree, 0, key, dep))
      return true;
    dep++;
  }
  return false;
}

// void p_dfs(Node *curNode, int key, bool *isFound, std::queue<Node *> *nodeQ)
// {
//   while (1) {
//     // std::cout << curNode << " " << curNode->key << std::endl;
//     if (*isFound)
//       return;
//     if (curNode->key == key) {
//       *isFound = true;
//       return;
//     }
//     for (size_t i = 1; i < curNode->numChildren; i++) {
//       (*nodeQ).push(curNode->children[i]);
//     }
//     if (curNode->numChildren >= 1) {
//       curNode = curNode->children[0];
//     } else
//       break;
//   }
// }

// void p_bfs(Node *curNode, int key, bool *isFound, std::queue<Node *>
// *nodeQ)
// {
//   // std::cout << curNode << " " << curNode->key << std::endl;
//   if (*isFound)
//     return;
//   if (curNode->key == key) {
//     *isFound = true;
//     return;
//   }
//   for (size_t i = 0; i < curNode->numChildren; i++) {
//     (*nodeQ).push(curNode->children[i]);
//   }
// }

// void p_sche(Node *root, int type, int key) {
//   bool isFound = false;
//   std::queue<Node *> nodeQ;
//   nodeQ.push(root);
//   while (1) {
//     if (isFound)
//       break;
//     if (!nodeQ.empty()) {
//       Node *t = nodeQ.front();
//       nodeQ.pop();
//       if (type == 0) {
//         std::async(p_bfs, t, key, &isFound, &nodeQ);
//       } else if (type == 1) {
//         std::async(p_dfs, t, key, &isFound, &nodeQ);
//       }
//     }
//   }
// }

bool p_bfs_omp(Tree *tree, int key) {
  std::vector<int> frontier;
  std::vector<int> next;

  frontier.push_back(0);

  bool isFound = false;
  int rfCnt = 0;
  while (!isFound && frontier.size() > 0) {
#pragma omp parallel
    {
#pragma omp for nowait
      for (size_t i = 0; i < frontier.size(); i++) {
        S_Node *s_node = tree->read_from_TS(frontier[i]);
        rfCnt++;
        if (s_node->key == key)
          isFound = true;
        if (isFound)
          continue;
#pragma omp critical
        // printf("%d\n", s_node->numChildren);
        next.insert(next.end(), s_node->children,
                    s_node->children + s_node->numChildren);
      }
    }
    frontier = next;
    next.clear();
  }
  printf("Read file count = %d\n", rfCnt);
  return isFound;
}

bool p_dfs_omp(Tree *tree, int offset, int key) {
  S_Node *s_node = tree->read_from_TS(offset);
  if (s_node->key == key)
    return true;

  bool isFound = false;

#pragma omp parallel for
  for (int i = 0; i < s_node->numChildren; i++) {
    if (!isFound && p_dfs_omp(tree, s_node->children[i], key))
      isFound = true;
  }
  return isFound;
}

bool p_iddfs_worker(Tree *tree, int offset, int key, size_t depLeft) {
  S_Node *s_node = tree->read_from_TS(offset);
  if (s_node->key == key)
    return true;
  if (depLeft == 0)
    return false;

  bool isFound = false;
#pragma omp parallel for
  for (int i = 0; i < s_node->numChildren; i++) {
    if (isFound)
      continue;
    if (p_iddfs_worker(tree, s_node->children[i], key, depLeft - 1))
      isFound = true;
  }
  return isFound;
}

bool p_iddfs_omp(Tree *tree, int key, size_t maxDepth) {
  bool isFound = false;
#pragma omp parallel for
  for (size_t i = 1; i <= maxDepth; i++) {
    if (isFound)
      continue;
    if (p_iddfs_worker(tree, 0, key, i))
      isFound = true;
  }
  return isFound;
}

bool p_mixed_omp(Tree *tree, int offset, int key) {
  std::vector<int> frontier;
  std::vector<int> next;

  frontier.push_back(offset);
  int cThres = 3;
  bool isFound = false;
  int rfCnt = 0;
  while (!isFound && frontier.size() > 0) {
#pragma omp parallel
    {
#pragma omp for nowait
      for (size_t i = 0; i < frontier.size(); i++) {
        S_Node *s_node = tree->read_from_TS(frontier[i]);
        rfCnt++;
        if (s_node->key == key)
          isFound = true;
        if (isFound)
          continue;
        if (s_node->numChildren > cThres) {
#pragma omp critical
          next.insert(next.end(), s_node->children,
                      s_node->children + s_node->numChildren);
        } else {
#pragma omp parallel for
          for (int i = 0; i < s_node->numChildren; i++) {
            if (isFound)
              continue;
            isFound |= p_mixed_omp(tree, s_node->children[i], key);
          }
        }
      }
    }
    frontier = next;
    next.clear();
  }
  // printf("Read file count = %d\n", rfCnt);
  return isFound;
}
