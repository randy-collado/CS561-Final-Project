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
  S_MetaData *s_metadata =
      (S_MetaData *)aligned_alloc(BLOCK_SIZE, sizeof(S_MetaData));
#endif
  s_metadata->numNode = numNode;
  s_metadata->maxDegree = maxDegree;
  s_metadata->dataType = 2;
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
  s_node->pSize = node->numValues;

  byte maxd = MAX_DEGREE - s_node->pSize;
  int edgeLeft = node->degree;
  int edgeOffset = 0;
  int cnt = 0;

  while (node->degree - edgeOffset > maxd) {
    if (maxd <= 0) {
      printf("Degree exceeded! Max size is (253-pSize)*253\n");
      exit(1);
    }
    maxd--; // Use one space for extra s_node

    // Create extra s_node
    S_Node *ext_s_node = new S_Node();
    ext_s_node->key = node->key;
    ext_s_node->degree = std::min(MAX_DEGREE, edgeLeft - maxd);
    ext_s_node->pSize = 0;
    for (auto i = 0; i < ext_s_node->degree; i++) {
      ext_s_node->data[i] = node->edges[edgeOffset + i]->id;
    }

    edgeOffset += ext_s_node->degree;
    edgeLeft -= ext_s_node->degree;

    // Link extra s_node to original s_node
    s_node->data[cnt++] = numNode + numExtSNode; // Id of extra block
    gs.writeNode(ext_s_node, numNode + numExtSNode);
    numExtSNode++;
  }
  for (auto i = edgeOffset; i < node->degree; ++i) {
    s_node->data[i - edgeOffset] = node->edges[i]->id;
  }
  s_node->degree = node->degree - edgeOffset;
  assert(s_node->degree + s_node->pSize <= MAX_DEGREE);
  std::memcpy(s_node->data + s_node->degree, node->values, node->numValues);

  return s_node;
}

S_Node *Graph::node_to_aligned_snode(GraphNode *node) {
#ifdef _WIN32
  S_Node *s_node = (S_Node *)_aligned_malloc(sizeof(S_Node), BLOCK_SIZE);
#else
  S_Node *s_node = (S_Node *)aligned_alloc(BLOCK_SIZE, sizeof(S_Node));
#endif

  s_node->key = node->key;
  s_node->pSize = node->numValues;

  byte maxd = MAX_DEGREE - s_node->pSize;
  int edgeLeft = node->degree;
  int edgeOffset = 0;
  int cnt = 0;

  while (node->degree - edgeOffset > maxd) {
    if (maxd <= 0) {
      printf("Degree exceeded! Max size is (253-pSize)*253\n");
      exit(1);
    }
    maxd--; // Use one space for extra s_node

    // Create extra s_node
#ifdef _WIN32
    S_Node *ext_s_node = (S_Node *)_aligned_malloc(sizeof(S_Node), BLOCK_SIZE);
#else
    S_Node *ext_s_node = (S_Node *)aligned_alloc(BLOCK_SIZE, sizeof(S_Node));
#endif
    ext_s_node->key = node->key;
    ext_s_node->degree = std::min(MAX_DEGREE, edgeLeft - maxd);
    ext_s_node->pSize = 0;
    for (auto i = 0; i < ext_s_node->degree; i++) {
      ext_s_node->data[i] = node->edges[edgeOffset + i]->id;
    }

    edgeOffset += ext_s_node->degree;
    edgeLeft -= ext_s_node->degree;

    // Link extra s_node to original s_node
    s_node->data[cnt++] = numNode + numExtSNode; // Id of extra block
    gs.writeNode(ext_s_node, numNode + numExtSNode);
    numExtSNode++;
  }
  for (auto i = edgeOffset; i < node->degree; ++i) {
    s_node->data[i - edgeOffset] = node->edges[i]->id;
  }
  s_node->degree = node->degree - edgeOffset;
  assert(s_node->degree + s_node->pSize <= MAX_DEGREE);
  std::memcpy(s_node->data + s_node->degree, node->values, node->numValues);

  return s_node;
}
