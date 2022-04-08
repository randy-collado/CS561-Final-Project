#include <windows.h>

#include "writeTree.h"

#include <iostream>
#include <queue>

void writeNodeOffset(HANDLE hFile, Node *p, int cnt) {
  std::vector<BYTE> buf(sizeof(Node));

  OVERLAPPED ol;
  ol.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  ol.Offset = cnt * sizeof(p->key);
  ol.OffsetHigh = 0;
  DWORD tf;
  printf("%d: %d\n", ol.Offset, p->key);
  while (!WriteFile(hFile, &p->key, sizeof(p->key), NULL, &ol)) {
    auto err = GetLastError();
    if (err == ERROR_IO_PENDING) {
      GetOverlappedResult(hFile, &ol, &tf, TRUE);
      break;
    }
  }
}

int writeTreeToFile(std::string fName, Node *root) {
  auto hFile = CreateFileA(fName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
  std::queue<Node *> Q;
  Q.push(root);
  int cnt = 0;
  while (!Q.empty()) {
    Node *p = Q.front();
    Q.pop();
    writeNodeOffset(hFile, p, cnt);
    cnt += 1;
    for (int i = 0; i < p->chs.size(); i++) {
      Q.push(p->chs[i]);
    }
  }
  return true;
}
