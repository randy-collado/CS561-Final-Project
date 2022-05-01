#include "algorithm.hpp"

bool s_dfs(Tree *tree, int offset, int &key) {
  S_Node *s_node = tree->read_from_TS(offset);
  if (s_node->key == key)
    return true;
  for (int i = 0; i < s_node->numChildren; i++) {
    if (s_dfs(tree, s_node->children[i], key))
      return true;
  }
  return false;
}

bool s_dfs_2(Tree *tree, int &key) {
  std::vector<int> frontier;
  frontier.push_back(0);

  while (!frontier.empty()) {
    S_Node *s_node = tree->read_from_TS(frontier.back());

    if (s_node->key == key)
      return true;

    frontier.pop_back();

    frontier.insert(frontier.end(), s_node->children,
                    s_node->children + s_node->numChildren);
  }
  return false;
}

bool _p_dfs_omp(Tree *tree, int &key, std::vector<int> &frontier, int &fSize) {
  bool isFound = false;

#pragma omp taskgroup
  while (!frontier.empty()) {
    if (isFound) {
      break;
    }
    S_Node *s_node = tree->read_from_TS(frontier.back());

    if (s_node->key == key) {
      isFound = true;
      break;
    }

    frontier.pop_back();

    frontier.insert(frontier.end(), s_node->children,
                    s_node->children + s_node->numChildren);
    while (frontier.size() > fSize) {
      if (isFound)
        break;
      std::vector<int> frontier_new(frontier.begin() + frontier.size() / 2,
                                    frontier.end());
      frontier.resize(frontier.size() / 2);
#pragma omp task shared(isFound)
      if (_p_dfs_omp(tree, key, frontier_new, fSize)) {
        isFound = true;
#pragma omp cancel taskgroup
      }
    }
  }
  return isFound;
}

bool p_dfs_omp(Tree *tree, int &key) {
  std::vector<int> initFrontier;
  int fSize = 16;

  initFrontier.push_back(0);
  bool isFound;
  std::cout << "Cancel " << omp_get_cancellation() << std::endl;
#pragma omp parallel
  {
#pragma omp single
    isFound = _p_dfs_omp(tree, key, initFrontier, fSize);
#pragma omp taskwait
  }
  return isFound;
}

bool s_iddfs_worker(Tree *tree, int offset, int &key, size_t depLeft) {
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

bool s_iddfs(Tree *tree, int &key, size_t maxDepth) {
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

bool s_bfs(Tree *tree, int &key) {
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
      if (isFound || s_node->numChildren == 0)
        continue;
      next.insert(next.end(), s_node->children,
                  s_node->children + s_node->numChildren);
    }
    frontier = next;
    next.clear();
  }
  printf("Read file count = %d\n", rfCnt);
  return isFound;
}

bool p_bfs_omp(Tree *tree, int &key) {
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
        if (isFound || s_node->numChildren == 0)
          continue;
#pragma omp critical
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

bool p_iddfs_worker(Tree *tree, int offset, int &key, size_t depLeft) {
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
    isFound |= p_iddfs_worker(tree, s_node->children[i], key, depLeft - 1);
  }
  return isFound;
}

bool p_iddfs_omp(Tree *tree, int &key, size_t maxDepth) {
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

bool p_hybrid_omp(Tree *tree, int offset, int &key, int &brhThres) {
  std::vector<int> frontier;
  std::vector<int> next;

  frontier.push_back(offset);
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
        if (isFound || s_node->numChildren == 0)
          continue;
        if (s_node->numChildren > brhThres) {
#pragma omp critical
          next.insert(next.end(), s_node->children,
                      s_node->children + s_node->numChildren);
        } else {
#pragma omp parallel for
          for (int i = 0; i < s_node->numChildren; i++) {
            if (isFound)
              continue;
            isFound |= p_hybrid_omp(tree, s_node->children[i], key, brhThres);
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

bool p_test_omp(Tree *tree, int offset, int &key, int &maxFSize) {
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
        if (isFound || s_node->numChildren == 0)
          continue;
#pragma omp critical
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
