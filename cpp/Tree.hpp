#include "Serializer.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <random>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#pragma once

struct TreeNode {
  TreeNode(size_t branchingFactor)
      : maxChildren(branchingFactor), childrenCount(0),
        maxValues(branchingFactor), numValues(0), level(0), key(-1) {}
  size_t maxChildren;
  size_t childrenCount;
  size_t maxValues;
  size_t numValues;
  size_t level;
  int key;
  int values[8];
  TreeNode *children[8];
  int number;
};

class Tree {
public:
  Tree(size_t branchingFactor)
      : nodeCount(0), head(nullptr), branch(branchingFactor), maxLevel(0),
        ts_init(false) {}

  ~Tree() {}

  void init_serializer(std::string filename, int rw);
  void init_metadata();

  void fill(long numInserts, long upperBound, std::vector<int> &keys,
            std::vector<int> &values);

  void seq_fill(long numInserts, std::vector<int> &keys,
            std::vector<int> &values);

  void add(int key, int value);

  void dump_tree();

  const TreeNode *get_head_ref() { return head; }

  size_t get_max_level() { return maxLevel; }

  S_Node *read_snode(int offset);
  S_MetaData* read_smetadata();

  size_t nodeCount;

private:
  void add_impl(int key, int value);

  void dump_node(TreeNode *head);

  S_Node *node_to_snode(TreeNode *node);
  S_Node *node_to_aligned_snode(TreeNode *node);

  TreeNode *head;
  size_t branch;
  size_t maxLevel;
  Serializer TS;
  bool ts_init;
};
