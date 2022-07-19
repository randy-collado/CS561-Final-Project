#include "Tree.hpp"
#include <omp.h>

void Tree::init_serializer(std::string filename, int rw) {
  if (rw == 0) {
    TS.openFile(filename, MODE::READ);
    ts_init = true;
  } else if (rw == 1) {
    TS.openFile(filename, MODE::WRITE);
    ts_init = true;
  } else {
    return;
  }
}

void Tree::dump_tree() {
  // Dump metadata
#ifdef _WIN32
  S_MetaData *s_metadata =
      (S_MetaData *)_aligned_malloc(sizeof(S_MetaData), BLOCK_SIZE);
#else
  S_MetaData *s_metadata =
      (S_MetaData *)aligned_alloc(BLOCK_SIZE, sizeof(S_MetaData));
#endif
  s_metadata->numNode = numNode;
  s_metadata->maxDegree = maxDegree;
  s_metadata->dataType = 1;
  TS.writeMetadata(s_metadata);
  // Dump node recursively
  dump_node(this->root);
}

void Tree::add(int key, int value) { add_impl(key, value); }

void Tree::fill(long numInserts, long upperBound, std::vector<int> &keys,
                std::vector<int> &values) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, upperBound);
  for (long i = 0; i < numInserts; ++i) {
    int key = distrib(gen);
    int value = distrib(gen);
    keys.push_back(key);
    values.push_back(value);
    add(key, value);
  }
}

void Tree::seq_fill(long numInserts, std::vector<int> &keys,
                    std::vector<int> &values) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 100);
  for (long i = 0; i < numInserts; ++i) {
    int key = i;
    int value = distrib(gen);
    keys.push_back(key);
    values.push_back(value);
    add(key, value);
  }
}

S_Node *Tree::read_snode(int id) {
  S_Node *s_node = TS.readNode(id);
  return s_node;
}

S_MetaData *Tree::read_smetadata() {
  S_MetaData *s_metadata = TS.readMetadata();
  return s_metadata;
}

void Tree::init_metadata() {
  S_MetaData *sm = read_smetadata();
  maxDegree = sm->maxDegree;
  numNode = sm->numNode;
}

void Tree::add_impl(int key, int value) {
  if (root == nullptr) {
    root = new TreeNode(maxDegree, maxValues);
    root->id = numNode++;
    root->key = key;
    root->values[root->numValues++] = value;
    root->level = 0;
    return;
  }

  std::vector<TreeNode *> frontier;
  std::vector<TreeNode *> next;

  frontier.push_back(root);

  while (frontier.size() > 0) {
    for (auto i = 0; i < frontier.size(); i++) {
      TreeNode *curNode = frontier[i];
      if (curNode->key == key) {
        if (curNode->numValues < curNode->maxValues)
          curNode->values[curNode->numValues++] = value;
        return;
      } else if (curNode->degree < curNode->maxDegree) {
        TreeNode *node = new TreeNode(maxDegree, maxValues);
        node->id = numNode++;
        node->key = key;
        node->values[node->numValues++] = value;
        node->level = curNode->level + 1;
        if (node->level > maxLevel) {
          maxLevel = node->level;
        }
        curNode->children[curNode->degree++] = node;
        return;
      }
      std::vector<TreeNode *> next_private(curNode->children,
                                           curNode->children + curNode->degree);
      next.insert(next.end(), next_private.begin(), next_private.end());
    }
    frontier = next;
    next.clear();
  }

  // std::deque<TreeNode *> queue = std::deque<TreeNode *>();
  // if (root == nullptr) {
  //   root = new TreeNode(branch);
  //   root->id = numNode++;
  //   root->key = key;
  //   root->values[root->numValues++] = value;
  //   root->level = 0;
  //   return;
  // }
  // queue.push_back(root);
  // while (!queue.empty()) {
  //   TreeNode *currentNode = queue.front();
  //   queue.pop_front();
  //   if (currentNode == nullptr) {
  //     assert(false && "unreachable");
  //   } else if (currentNode->key == key) {
  //     if (currentNode->numValues < currentNode->maxValues)
  //       currentNode->values[currentNode->numValues++] = value;
  //     return;
  //   } else if (currentNode->degree < branch) {
  //     TreeNode *node = new TreeNode(branch);
  //     node->id = numNode++;
  //     node->key = key;
  //     node->values[node->numValues++] = value;
  //     node->level = currentNode->level + 1;
  //     if (node->level > maxLevel) {
  //       maxLevel = node->level;
  //     }
  //     currentNode->edges[currentNode->degree++] = node;
  //     return;
  //   } else {
  //     queue.insert(queue.end(), currentNode->edges,
  //                  currentNode->edges + currentNode->degree);
  //   }
  // }
}

void Tree::dump_node(TreeNode *root) {
  if (!ts_init) {
    std::cerr << "[ERROR] Serializer was not initialized" << std::endl;
    exit(1);
  }
  if (!root)
    return;

  auto s_node = node_to_aligned_snode(root);
  TS.writeNode(s_node, root->id);

#pragma omp parallel for
  for (auto i = 0; i < root->degree; i++) {
    dump_node(root->children[i]);
  }

  // std::deque<TreeNode *> queue = std::deque<TreeNode *>();
  // queue.push_back(root);

  // while (!queue.empty()) {
  //   TreeNode *currentNode = queue.front();
  //   queue.pop_front();
  //   if (currentNode == nullptr)
  //     assert(false && "Unreachable");

  //   // Aligned
  //   auto s_node_ptr = node_to_aligned_snode(currentNode);

  //   TS.writeNodeWithOffset(s_node_ptr, BLOCK_SIZE * currentNode->id);

  //   queue.insert(queue.end(), currentNode->edges,
  //                currentNode->edges + currentNode->degree);
  // }
}

S_Node *Tree::node_to_snode(
    TreeNode *node) { // returns a pointer declared by new, user should delete
  S_Node *s_node = new S_Node();

  assert(node->degree + node->numValues <= MAX_DEGREE);

  s_node->key = node->key;
  s_node->degree = node->degree;
  s_node->pSize = node->numValues;

  for (auto i = 0; i < node->degree; ++i) {
    s_node->data[i] = node->children[i]->id;
  }
  std::memcpy(s_node->data + s_node->degree, node->values, node->numValues);

  return s_node;
}

S_Node *Tree::node_to_aligned_snode(TreeNode *node) {
#ifdef _WIN32
  S_Node *s_node = (S_Node *)_aligned_malloc(sizeof(S_Node), BLOCK_SIZE);
#else
  S_Node *s_node = (S_Node *)aligned_alloc(BLOCK_SIZE, sizeof(S_Node));
#endif
  assert(node->degree + node->numValues <= MAX_DEGREE);

  s_node->key = node->key;
  s_node->degree = node->degree;
  s_node->pSize = node->numValues;

  for (auto i = 0; i < node->degree; ++i) {
    s_node->data[i] = node->children[i]->id;
  }
  std::memcpy(s_node->data + s_node->degree, node->values, node->numValues);

  return s_node;
}
