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
  ~GraphNode();
  int key, number;
  int degree, numValues;
  std::vector<GraphNode *> edges;
  int values[8];
};

class Graph {
public:
  Graph() : numNode(0), maxDegree(0), gs_init(false) {}
  ~Graph();

  void init_serializer(std::string filename, int rw);
  void init_metadata();

  void dump_graph();

  S_Node *read_snode(int number);
  S_MetaData *read_smetadata();

  int numNode;
  int maxDegree;

private:

  void dump_node(GraphNode *node);

  S_Node *node_to_snode(GraphNode *node);
  S_Node *node_to_aligned_snode(GraphNode *node);

  std::vector<GraphNode> nodes;
  Serializer gs;
  bool gs_init;
};
