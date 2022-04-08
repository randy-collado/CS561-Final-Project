#include <vector>

class Node {
public:
  int key;
  std::vector<Node *> chs;
  std::vector<BYTE> data;
};
