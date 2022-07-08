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

struct S_Node {
  int key;                       // 4 Bytes
  int payloads[8];               // 32 Bytes
  unsigned short degree;         // 2 Bytes
  unsigned short edges[237];     // 512 - 38 bytes, offset of neighbor nodes
} __attribute__((aligned(512))); // GCC extension to align a struct

struct S_Node_2 {
  unsigned short degree;     // 2 Bytes
  unsigned short edges[255]; // 255 * 2 = 510 bytes
} __attribute__((aligned(512)));

struct S_MetaData {
  int numNode;
  int maxDegree;
} __attribute__((aligned(512)));

class Serializer {
public:
  Serializer();
  ~Serializer();

  bool writeMetadata(S_MetaData *metadata);
  S_MetaData *readMetadata();

  bool writeNode(S_Node *node, int number);
  bool writeNodeWithOffset(S_Node *node, size_t offset);

  S_Node *readNode(int number);
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
