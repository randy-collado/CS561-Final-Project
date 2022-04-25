#include "TreeSerializer.hpp"
#include "Tree.hpp"

#define align(p, a) (((long)(p) + (a - 1)) & ~(a - 1))

TreeSerializer::TreeSerializer()
    : mode_internal(MODE::INVALID), offset(0), at_eof(false),
      read_offset(sizeof(tier_offsets)) {
#ifndef _WIN32
  fd = -1;
#endif
}

TreeSerializer::~TreeSerializer() {
#ifdef _WIN32
  CloseHandle(hf);
#else
  close(fd);
#endif
}

void TreeSerializer::openFile(std::string filepath, MODE mode) {
#ifdef _WIN32
  DWORD dwflags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING;
  switch (mode) {
  case MODE::READ: // READ
    hf = CreateFileA(filepath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING,
                     dwflags, NULL);
    if (hf == INVALID_HANDLE_VALUE) {
      printf("Failed to create file.\n");
      exit(1);
    }
    this->mode_internal = mode;
    break;
  case MODE::WRITE: // WRITE
    hf = CreateFileA(filepath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
                     CREATE_ALWAYS, dwflags, NULL);
    if (hf == INVALID_HANDLE_VALUE) {
      printf("Failed to create file.\n");
      exit(1);
    }
    this->mode_internal = mode;
    break;
  default:
    break;
  }
#else
  int flags = 0;
  switch (mode) {
  case MODE::READ: // READ
    flags |= O_RDONLY | O_DIRECT;
    this->mode_internal = mode;
    break;
  case MODE::WRITE: // WRITE
    flags |= O_RDWR | O_CREAT | O_DIRECT;
    this->mode_internal = mode;
    break;
  default:
    break;
  }
  this->fd = open(filepath.c_str(), flags);
  if (this->fd < 0) {
    std::cerr << "[ERROR]: Serializer failed\n[MESSAGE]: file unable to be "
                 "opened or created";
    exit(1);
  }
#endif
}

void TreeSerializer::writeNode(S_Node *node) {
  if (mode_internal == MODE::READ || mode_internal == MODE::INVALID) {
    std::cerr << "[ERROR]: Serializer in an invalid state\n[REASON]: Must be "
                 "initialized to write mode via MODE::WRITE"
              << std::endl;
    exit(1);
  }

  // printf("Offset: %lld\n", offset);

#ifdef _WIN32
  OVERLAPPED ol;
  ol.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  ol.Offset = offset;
  ol.OffsetHigh = 0;
  DWORD bytes;

  // std::cout << node << std::endl;
  // printf("Node size: %lld\n", sizeof(S_Node));
  if (!WriteFile(hf, (char *)node, sizeof(S_Node), &bytes, &ol)) {
    auto err = GetLastError();
    printf("Err: %ld\n", err);
    exit(1);
  }
  // printf("Bytes written: %ld\n", bytes);
#else
  ssize_t bytes = pwrite(fd, (char *)node, sizeof(S_Node), offset);
  printf("Bytes written: %ld\n", bytes);
#endif
  if (bytes <= 0) {
    std::cout << "[ERROR]: Could not write\n[MESSAGE]: " +
                     std::string(strerror(errno))
              << std::endl;
    return;
  }
  // Update offsets
  this->offset += bytes;
}

S_Node *TreeSerializer::readNode() {
  if (at_eof) {
    std::cerr << "[ERROR]: Could not read\n[MESSAGE]: EOF has been reached"
              << std::endl;
    return nullptr;
  }
  S_Node *node = new S_Node();
#ifdef _WIN32
  OVERLAPPED ol;
  ol.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  ol.Offset = read_offset;
  ol.OffsetHigh = 0;
  DWORD bytes;
  if (!ReadFile(hf, node, sizeof(S_Node), &bytes, &ol)) {
    auto err = GetLastError();
    printf("Err: %ld\n", err);
    exit(1);
  }
  // printf("Read %ld\n", bytes);
#else
  ssize_t bytes = pread(fd, (char *)node, sizeof(S_Node), read_offset);
#endif
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
#ifdef _WIN32
  read_offset = offset;
#else
  off_t bytes = lseek(fd, 0, SEEK_SET);
  bytes = lseek(fd, sizeof(tier_offsets), SEEK_SET);
  bytes = lseek(fd, offset, SEEK_CUR);
#endif
  return readNode();
}

/*
 *
 */
void TreeSerializer::write_offset_metadata(tier_offsets *metadata) {
#ifdef _WIN32
  OVERLAPPED ol;
  ol.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  ol.Offset = 0;
  ol.OffsetHigh = 0;
  DWORD bytes;
  if (!WriteFile(hf, metadata, sizeof(tier_offsets), &bytes, &ol)) {
    auto err = GetLastError();
    printf("Err: %ld\n", err);
    exit(1);
  }
#else
  ssize_t bytes = pwrite(fd, (char *)metadata, sizeof(tier_offsets), 0);
#endif
  if (bytes <= 0)
    std::cout << "[ERROR]: Could not write metadata\n[MESSAGE]: " +
                     std::string(strerror(errno))
              << std::endl;
}

tier_offsets *TreeSerializer::read_offset_metadata() {
  tier_offsets *metadata = new tier_offsets();
#ifdef _WIN32
  OVERLAPPED ol;
  ol.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  ol.Offset = 0;
  ol.OffsetHigh = 0;
  DWORD bytes;
  if (!ReadFile(hf, metadata, sizeof(tier_offsets), &bytes, &ol)) {
    auto err = GetLastError();
    printf("Err: %ld\n", err);
    exit(1);
  }
#else
  ssize_t bytes = pread(fd, (char *)metadata, sizeof(tier_offsets), 0);
#endif
  if (bytes <= 0) {
    std::cout << "[ERROR]: Could not read metadata\n[MESSAGE]: " +
                     std::string(strerror(errno))
              << std::endl;
    return nullptr;
  }
  return metadata;
}

size_t TreeSerializer::get_current_offset() { return offset; }
