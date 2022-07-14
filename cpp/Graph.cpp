#include "Graph.hpp"
#include <omp.h>

void Graph::init_serializer(std::string filename, int rw) {
  if (rw == 0) {
    gs.openFile(filename, MODE::READ);
    gs_init = true;
  } else if (rw == 1) {
    gs.openFile(filename, MODE::WRITE);
    gs_init = true;
  } else {
    return;
  }
}

void Graph::dump_graph() {
  // Dump metadata
#ifdef _WIN32
  S_MetaData *s_metadata =
      (S_MetaData *)_aligned_malloc(sizeof(S_MetaData), BLOCK_SIZE);
#else
  S_MetaData *s_metadata = (S_MetaData *)aligned_alloc(BLOCK_SIZE, sizeof(S_MetaData));
#endif
  s_metadata->numNode = numNode;
  s_metadata->maxDegree = maxDegree;
  gs.writeMetadata(s_metadata);

#pragma omp parallel for
  // Dump each node
  for (int i = 0; i < numNode; i++) {
    dump_node(&nodes[i]);
  }
}

S_Node *Graph::read_snode(int id) {
  S_Node *s_node = gs.readNode(id);
  return s_node;
}

S_MetaData *Graph::read_smetadata() {
  S_MetaData *s_metadata = gs.readMetadata();
  return s_metadata;
}

void Graph::init_metadata() {
  S_MetaData *sm = read_smetadata();
  maxDegree = sm->maxDegree;
  numNode = sm->numNode;
}

void Graph::dump_node(GraphNode *node) {
  if (!gs_init) {
    std::cerr << "[ERROR] Serializer was not initialized" << std::endl;
    exit(1);
  }
  if (!node)
    return;

  auto s_node = node_to_aligned_snode(node);
  gs.writeNode(s_node, node->id);
}

S_Node *Graph::node_to_snode(GraphNode *node) {
  S_Node *s_node = new S_Node();
  s_node->key = node->key;
  s_node->degree = node->degree;

  std::memcpy(s_node->payloads, node->values, node->numValues);

  if (node->degree <= MAX_DEGREE) {
    for (auto i = 0; i < node->degree; ++i) {
      s_node->edges[i] = node->edges[i]->id;
    }
  }

  return s_node;
}

S_Node *Graph::node_to_aligned_snode(GraphNode *node) {
#ifdef _WIN32
  S_Node *s_node = (S_Node *)_aligned_malloc(sizeof(S_Node), BLOCK_SIZE);
#else
  S_Node *s_node = (S_Node *)aligned_alloc(BLOCK_SIZE, sizeof(S_Node));
#endif

  s_node->key = node->key;
  s_node->degree = node->degree;

  std::memcpy(s_node->payloads, node->values, node->numValues);

  if (node->degree <= MAX_DEGREE) {
    for (auto i = 0; i < node->degree; ++i) {
      s_node->edges[i] = node->edges[i]->id;
    }
  }

  return s_node;
}
