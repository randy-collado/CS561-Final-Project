#include "TreeSerializer.hpp"
#include "Tree.hpp"

#define align(p, a) (((long)(p) + (a - 1)) & ~(a - 1))

TreeSerializer::TreeSerializer() : mode_internal(MODE::INVALID), at_eof(false) {
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
  DWORD dwflags =
      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED;
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
  this->fd = open(filepath.c_str(), flags, 0777);
  if (this->fd < 0) {
    std::cerr << "[ERROR]: Serializer failed\n[MESSAGE]: file unable to be "
                 "opened or created";
    exit(1);
  }
#endif
}

void TreeSerializer::writeNodeWithOffset(S_Node *node, int offset) {
  if (mode_internal == MODE::READ || mode_internal == MODE::INVALID) {
    std::cerr << "[ERROR]: Serializer in an invalid state\n[REASON]: Must be "
                 "initialized to write mode via MODE::WRITE"
              << std::endl;
    exit(1);
  }

#ifdef _WIN32
  OVERLAPPED ol;
  ol.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  ol.Offset = offset;
  ol.OffsetHigh = 0;
  DWORD bytes;

  if (!WriteFile(hf, (char *)node, sizeof(S_Node), NULL, &ol)) {
    auto err = GetLastError();
    if (err != ERROR_IO_PENDING) {
      printf("Err: %ld\n", err);
      exit(1);
    } else {
      if (!GetOverlappedResult(hf, &ol, &bytes, TRUE)) {
        printf("Err: %ld\n", err);
        exit(1);
      }
    }
  }
  // printf("Bytes written: %ld\n", bytes);
#else
  ssize_t bytes = pwrite(fd, (char *)node, sizeof(S_Node), offset);
  // printf("Bytes written: %ld\n", bytes);
#endif
  if (bytes <= 0) {
    std::cout << "[ERROR]: Could not write\n[MESSAGE]: " +
                     std::string(strerror(errno))
              << std::endl;
    return;
  }
}

S_Node *TreeSerializer::readNode(size_t offset) {
  if (at_eof) {
    std::cerr << "[ERROR]: Could not read\n[MESSAGE]: EOF has been reached"
              << std::endl;
    return nullptr;
  }
  S_Node *node = new S_Node();
#ifdef _WIN32
  OVERLAPPED ol;
  ol.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  ol.Offset = offset;
  ol.OffsetHigh = 0;
  DWORD bytes;
  if (!ReadFile(hf, node, sizeof(S_Node), &bytes, &ol)) {
    auto err = GetLastError();
    if (err != ERROR_IO_PENDING) {
      printf("Err: %ld\n", err);
      exit(1);
    } else {
      if (!GetOverlappedResult(hf, &ol, &bytes, TRUE)) {
        printf("Err: %ld\n", err);
        exit(1);
      }
    }
  }
#else
    // int ret;
    // char* buf;
    // ret = posix_memalign((void **)&buf, 512, 512);
    // if (ret) {
    //     perror("posix_memalign failed");
    //     exit(1);
    // }
    // ssize_t bytes = pread(fd, buf, sizeof(S_Node), offset);
  ssize_t bytes = pread(fd, (char *)node, sizeof(S_Node), offset);
  // node = (S_Node*) buf;
  // std::cout << "Read Node" << " Offset " << offset << " Key: " << node->key << " +++" << std::endl;
#endif
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
  if ((offset % S_NODE_SIZE) != 0) {
    return nullptr;
  }
  return readNode(offset);
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
    if (err != ERROR_IO_PENDING) {
      printf("Err: %ld\n", err);
      exit(1);
    } else {
      if (!GetOverlappedResult(hf, &ol, &bytes, TRUE)) {
        printf("Err: %ld\n", err);
        exit(1);
      }
    }
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
    if (err != ERROR_IO_PENDING) {
      printf("Err: %ld\n", err);
      exit(1);
    } else {
      if (!GetOverlappedResult(hf, &ol, &bytes, TRUE)) {
        printf("Err: %ld\n", err);
        exit(1);
      }
    }
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

// size_t TreeSerializer::get_current_offset() { return offset; }
