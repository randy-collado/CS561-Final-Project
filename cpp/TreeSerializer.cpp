#include "TreeSerializer.hpp"
#include "Tree.hpp"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define align(p, a) (((long)(p) + (a - 1)) & ~(a - 1))

TreeSerializer::TreeSerializer()
    : at_eof(false), fd(-1), mode_internal(MODE::INVALID),
      read_offset(sizeof(tier_offsets)), offset(0) {}

TreeSerializer::~TreeSerializer() { close(fd); }

void TreeSerializer::openFile(std::string filepath, MODE mode) {
  int flags = 0;
  switch (mode) {
  case MODE::READ: // READ
    flags |= O_RDONLY;
    this->mode_internal = mode;
    break;
  case MODE::WRITE: // WRITE
    flags |= O_RDWR | O_CREAT;
    this->mode_internal = mode;
    break;
  default:
    break;
  }
  this->fd = open(filepath.c_str(), O_RDWR | O_CREAT);
  if (this->fd < 0) {
    std::cerr << "[ERROR]: Serializer failed\n[MESSAGE]: file unable to be "
                 "opened or created";
    exit(1);
  }
}

void TreeSerializer::writeNode(S_Node *node) {
  if (mode_internal == MODE::READ || mode_internal == MODE::INVALID) {
    std::cerr << "[ERROR]: Serializer in an invalid state\n[REASON]: Must be "
                 "initialized to write mode via MODE::WRITE"
              << std::endl;
    exit(1);
  }
  ssize_t bytes = pwrite(fd, (char *)node, sizeof(S_Node), offset);
  std::cout << "Bytes written: " << bytes << std::endl;
  if (bytes <= 0) {
    std::cout << "[ERROR]: Could not write\n[MESSAGE]: " +
                     std::string(strerror(errno))
              << std::endl;
    return;
  }
  this->offset += bytes;
}

S_Node *TreeSerializer::readNode() {
  if (at_eof) {
    std::cerr << "[ERROR]: Could not read\n[MESSAGE]: EOF has been reached"
              << std::endl;
    return nullptr;
  }
  S_Node *node = new S_Node();
  ssize_t bytes = pread(fd, (char *)node, sizeof(S_Node), read_offset);
  //		std::cout << "read node " << node->id << std::endl;
  //		std::cout << "Bytes read: " << bytes << std::endl;
  if (bytes <= 0) {
    std::cout << "[ERROR]: Could not read\n[MESSAGE]: " +
                     std::string(strerror(errno))
              << std::endl;
    at_eof = true;
  }
  return node;
}
/*
 * The parameters of this function should be a multiple of 512, the size of a
 * node. Will return nullptr if the offset is not 512-byte aligned.
 *
 * Might be useful to allow this to return pointers to individual members of a
 * struct, but casting might be needed in this case
 */
S_Node *TreeSerializer::readNodeFromOffset(size_t offset) {
  if (!(offset % S_NODE_SIZE)) {
    return nullptr;
  }
  off_t bytes = lseek(fd, 0, SEEK_SET);
  bytes = lseek(fd, sizeof(tier_offsets), SEEK_SET);
  bytes = lseek(fd, offset, SEEK_CUR);
  return readNode();
}

/*
 *
 */
void TreeSerializer::write_offset_metadata(tier_offsets *metadata) {
  ssize_t bytes = pwrite(fd, (char *)metadata, sizeof(tier_offsets), 0);
  if (bytes <= 0)
    std::cout << "[ERROR]: Could not write metadata\n[MESSAGE]: " +
                     std::string(strerror(errno))
              << std::endl;
}

tier_offsets *TreeSerializer::read_offset_metadata() {
  tier_offsets *metadata = new tier_offsets();
  ssize_t bytes = pread(fd, (char *)metadata, sizeof(tier_offsets), 0);
  if (bytes <= 0) {
    std::cout << "[ERROR]: Could not read metadata\n[MESSAGE]: " +
                     std::string(strerror(errno))
              << std::endl;
    return nullptr;
  }
  return metadata;
}

size_t TreeSerializer::get_current_offset() { return offset; }
