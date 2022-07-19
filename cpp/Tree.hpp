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

#define BLOCK_SIZE 512
#define MAX_DEGREE 253

struct TreeNode {
  TreeNode(int maxDegree, int maxValues)
      : key(-1), maxDegree(maxDegree), degree(0), maxValues(maxValues),
        numValues(0), level(0) {
    children = new TreeNode *[maxDegree];
    values = new u_short[maxValues];
  }
  ~TreeNode() { delete[] children; }
  int id, key;
  int maxDegree, degree;
  byte maxValues, numValues;
  int level;
  u_short *values;
  TreeNode **children;
};

class Tree {
public:
  Tree(int maxDegree, int maxValues)
      : numNode(0), root(nullptr), maxDegree(maxDegree), maxLevel(0),
        maxValues(maxValues), ts_init(false) {}

  ~Tree() { delete root; }

  void init_serializer(std::string filename, int rw);
  void init_metadata();

  void fill(long numInserts, long upperBound, std::vector<int> &keys,
            std::vector<int> &values);

  void seq_fill(long numInserts, std::vector<int> &keys,
                std::vector<int> &values);

  void add(int key, int value);

  void dump_tree();

  const TreeNode *get_head_ref() { return root; }

  int get_max_level() { return maxLevel; }

  S_Node *read_snode(int id);
  S_MetaData *read_smetadata();

  int numNode;

private:
  void add_impl(int key, int value);

  void dump_node(TreeNode *root);

  S_Node *node_to_snode(TreeNode *node);
  S_Node *node_to_aligned_snode(TreeNode *node);

  TreeNode *root;
  int maxDegree;
  int maxLevel;
  int maxValues;
  Serializer TS;
  bool ts_init;
};
