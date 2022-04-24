#include <string>

#pragma once

#define S_NODE_SIZE 512

enum MODE { READ, WRITE, INVALID };

struct dummy_node {
  int x;
  int y;
  int z;
};

struct tier_offsets {
  int tiers;
  int offsets[511];
};

struct S_Node {
  int id;
  int payloads[8];
  int children[8];               // id or keys of the nodes
} __attribute__((aligned(512))); // GCC extension to align a struct

class TreeSerializer {
public:
  TreeSerializer();
  ~TreeSerializer();
  void writeNode(S_Node *node);
  void openFile(std::string filepath, MODE mode);
  S_Node *readNode();
  S_Node *readNodeFromOffset(size_t offset);
  size_t get_current_offset();
  void write_offset_metadata(tier_offsets *metadata);
  tier_offsets *read_offset_metadata();

private:
  int fd;
  MODE mode_internal;
  size_t offset;
  bool at_eof;
  size_t read_offset;
};
