#include "TreeSerializer.hpp"
#include <cassert>
#include <cstring>
#include <deque>
#include <iostream>
#include <random>
#include <vector>

#pragma once

struct Node {
  Node(size_t branchingFactor)
      : maxChildren(branchingFactor), numChildren(0),
        maxValues(branchingFactor), numValues(0), key(-1) {}
  size_t maxChildren;
  size_t numChildren;
  size_t maxValues;
  size_t numValues;
  int key;
  int values[8];
  Node *children[8];
};

class Tree {
public:
  Tree(size_t branchingFactor)
      : branch(branchingFactor), head(nullptr), ts_init(false) {}

  ~Tree() {}

  void init_serializer(std::string filename);
  S_Node *node_to_snode(Node *node);
  void fill(long numInserts, long upperBound, std::vector<int> &keys,
            std::vector<int> &values);
  void add(int key, int value);
  void dump_tree();

  const Node *get_head_ref() { return head; }

  S_Node *digestNode() { return TS.readNode(); }

private:
  void add_impl(int key, int value);
  void dump_tree_impl(Node *head);
  Node *head;
  size_t numElements;
  size_t branch;
  TreeSerializer TS;
  bool ts_init;
};
