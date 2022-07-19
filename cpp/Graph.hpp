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

#define MAX_DEGREE 253
#define BLOCK_SIZE 512

struct GraphNode {
  GraphNode(byte numValues = 0)
      : key(-1), id(0), degree(0), numValues(numValues) {
    values = new u_short[numValues];
  }
  ~GraphNode();
  int key, id;
  int degree;
  byte numValues;
  std::vector<GraphNode *> edges;
  u_short *values;
};

class Graph {
public:
  Graph() : numNode(0), maxDegree(0), numExtSNode(0), gs_init(false) {}
  ~Graph();

  void init_serializer(std::string filename, int rw);
  void init_metadata();

  void dump_graph();

  S_Node *read_snode(int id);
  S_MetaData *read_smetadata();

  int numNode;
  int maxDegree;
  int numExtSNode;

private:
  void dump_node(GraphNode *node);

  S_Node *node_to_snode(GraphNode *node);
  S_Node *node_to_aligned_snode(GraphNode *node);

  std::vector<GraphNode> nodes;
  Serializer gs;
  bool gs_init;
};
