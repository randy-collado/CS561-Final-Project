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

struct GraphNode {
  GraphNode() : degree(0), numValues(0), key(-1) {}
  ~GraphNode() {
    for (int i = 0; i < 8; i++)
      delete children[i];
  }
  int key;
  int values[8];
  int degree;
  size_t numValues;
  GraphNode *children[8];
  int number;
};

class Graph {
public:
  Graph() : numNode(0), root(nullptr), gs_init(false) {}

  ~Graph() { delete root; }

  void init_serializer(std::string filename, int rw);
  void init_metadata();

  void add(int key, int value);

  void dump_graph();

  S_Node *read_snode(int offset);
  S_MetaData *read_smetadata();

  size_t numNode;

private:
  void add_impl(int key, int value);

  void dump_node(GraphNode *root);

  S_Node *node_to_snode(GraphNode *node);
  S_Node *node_to_aligned_snode(GraphNode *node);

  GraphNode *root;
  Serializer gs;
  bool gs_init;
};
