#include "TreeSerializer.hpp"
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

struct Node {
  Node(size_t branchingFactor)
      : maxChildren(branchingFactor), numChildren(0),
        maxValues(branchingFactor), numValues(0), level(0), key(-1) {}
  size_t maxChildren;
  size_t numChildren;
  size_t maxValues;
  size_t numValues;
  size_t level;
  int key;
  int values[8];
  Node *children[8];
};

class Tree {
public:
  Tree(size_t branchingFactor)
      : head(nullptr), branch(branchingFactor), maxLevel(0), ts_init(false) {}

  ~Tree() {}

  void init_serializer(std::string filename);
  S_Node *node_to_snode(Node *node);
  void fill(long numInserts, long upperBound, std::vector<int> &keys,
            std::vector<int> &values);
  void add(int key, int value);
  void dump_tree();
  void dump_tree_tiered() { dump_tree_tiered_impl(this->head); }

  Node *get_head_ref() { return head; }

  S_Node *digestNode() { return TS.readNode(); }

  tier_offsets digest_metadata() { return *TS.read_offset_metadata(); }

  size_t get_max_level() { return maxLevel; }

private:
  void add_impl(int key, int value);

  void dump_tree_impl(Node *head);

  void dump_tree_tiered_impl(Node *head);

  S_Node *node_to_aligned_snode(Node *node);
  Node *head;
  size_t numElements;
  size_t branch;
  size_t maxLevel;
  TreeSerializer TS;
  bool ts_init;
};
