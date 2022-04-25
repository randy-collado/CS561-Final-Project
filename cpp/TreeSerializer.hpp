#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#endif

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
  int key;
  int numChildren;
  int payloads[8];
  int children[8];               // offset of children nodes
} __attribute__((aligned(512))); // GCC extension to align a struct

class TreeSerializer {
public:
  TreeSerializer();
  ~TreeSerializer();
  void writeNodeWithOffset(S_Node *node, int offset);
  void openFile(std::string filepath, MODE mode);
  S_Node *readNode();
  S_Node *readNodeFromOffset(size_t offset);
  // size_t get_current_offset();
  void write_offset_metadata(tier_offsets *metadata);
  tier_offsets *read_offset_metadata();

private:
#ifdef _WIN32
  HANDLE hf;
#else
  int fd;
#endif
  MODE mode_internal;
  size_t write_offset;
  bool at_eof;
  size_t read_offset;
};
