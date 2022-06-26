#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#endif

#pragma once

#define S_NODE_SIZE 512
#define S_METADATA_SIZE 512

enum MODE { READ, WRITE, INVALID };

struct dummy_node {
  int x;
  int y;
  int z;
};

struct S_Node {
  int key;
  int numChildren;
  int payloads[8];
  int children[8];               // offset of children nodes
} __attribute__((aligned(512))); // GCC extension to align a struct

struct S_MetaData {
  int cntNode;
  int maxBranch;
} __attribute__((aligned(512)));

class TreeSerializer {
public:
  TreeSerializer();
  ~TreeSerializer();

  void writeMetadata(S_MetaData *metadata);
  S_MetaData *readMetadata();

  void writeNodeWithOffset(S_Node *node, int offset);
  S_Node *readNodeFromOffset(size_t offset);

  void openFile(std::string filepath, MODE mode);
  

private:
#ifdef _WIN32
  HANDLE hf;
#else
  int fd;
#endif

  MODE mode_internal;
  bool at_eof;
};
