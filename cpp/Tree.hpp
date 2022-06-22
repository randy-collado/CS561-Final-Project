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
  int number;
};

class Tree {
public:
  Tree(size_t branchingFactor)
      : nodeCount(0), head(nullptr), branch(branchingFactor), maxLevel(0),
        ts_init(false) {}

  ~Tree() {}

  void init_serializer(std::string filename, int rw);

  void fill(long numInserts, long upperBound, std::vector<int> &keys,
            std::vector<int> &values);

  void seq_fill(long numInserts, std::vector<int> &keys,
            std::vector<int> &values);

  void add(int key, int value);

  void dump_tree();
  // void dump_tree_tiered() { dump_tree_tiered_impl(this->head); }

  const Node *get_head_ref() { return head; }

  // S_Node *digestNode() { return TS.readNode(); }

  tier_offsets digest_metadata() { return *TS.read_offset_metadata(); }

  size_t get_max_level() { return maxLevel; }

  S_Node *read_from_TS(int offset);

  size_t nodeCount;

private:
  void add_impl(int key, int value);

  void dump_tree_impl(Node *head);

  void dump_tree_tiered_impl(Node *head);

  S_Node *node_to_snode(Node *node);
  S_Node *node_to_aligned_snode(Node *node);

  // Node *snode_to_node(S_Node *s_node);

  Node *head;

  size_t branch;
  size_t maxLevel;
  TreeSerializer TS;
  bool ts_init;
};
